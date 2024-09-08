module;
#include <string>

#include <CLI/CLI.hpp>
#include <Eigen/Core>
#include <fmt/format.h>
#include <fmt/os.h>

module zjucad.gauwn_cli.spgrad;

import zjucad.gauwn.io.vtk;
import zjucad.gauwn.io.obj;
import zjucad.gauwn.utils;
import zjucad.gauwn.utils.grid2d;
import zjucad.gauwn.utils.topology;
import zjucad.gauwn.piecewise2d.gradient;
import zjucad.gauwn.meshwise2d.aabb;
import zjucad.gauwn.bvh2d.aabb;
import zjucad.gauwn.bvh2d.approx_polygon;

import zjucad.gauwn_cli.cli_subcmd;

using namespace zjucad::gauwn;

namespace zjucad::gauwn_cli {
Subcmd cmd_spgrad(CLI::App *app, SubcmdContext *ctx) {
    std::string input_obj;
    std::string output_vtk;
    int resolution = 100;
    int repeat = 1;
    double sigma = 0.01;
    int split_limit = 4;
    double beta = 8.0;

    app->add_option("--input-obj", input_obj, "Input obj file")->required();
    app->add_option("--output-vtk", output_vtk, "Output vtk file")->required();
    app->add_option("--resolution", resolution, "Resolution of the grid");
    app->add_option("--sigma", sigma, "Sigma for Gaussian function")->required();
    app->add_option("--repeat", repeat, "Repeat for timing");
    app->add_option("--split-limit", split_limit, "Split limit for AABB tree");
    app->add_option("--beta", beta, "beta");

    app->require_subcommand(1);

    bvh2d::AABBTree tree;
    bvh2d::AABBApproxPolygon poly;
    utils::VertexCellAdj adj;
    Eigen::MatrixXd V;
    Eigen::MatrixXi L;
    Eigen::MatrixXd gridV;
    Eigen::MatrixXi gridC;
    Eigen::MatrixXd value;

    auto run = [&](auto &&f) {
        double t = utils::time([&] {
#pragma omp parallel for
            for (int i = 0; i < gridV.rows(); i++) {
                Eigen::Vector2d t = gridV.row(i);
                Eigen::Vector2d result;
                result.setZero();
                f(t, 1.0, result);
                value.row(i) = result;
            }
        },
                               repeat);
        fmt::print("Time: {}\n", t);
    };

    auto gauwn = [&](CLI::App *app, SubcmdContext *ctx) -> Subcmd {
        co_await dispatch_cmd;
        piecewise2d::Gradient grad_eval_pw{.sigma = sigma};

        meshwise2d::SpatialGradientAABB<piecewise2d::Gradient> grad_eval_mw_aabb{
            .gradient = grad_eval_pw, .V = &V, .L = &L, .adj = &adj, .tree = &tree, .poly = &poly, .beta = beta};

        run(grad_eval_mw_aabb);
        co_await dispatch_cmd;
    };
    ctx->subcmd(app->add_subcommand("gauwn"), gauwn);

    co_await dispatch_cmd;

    if (!zjucad::gauwn::io::read_obj2d(input_obj, V, L)) {
        fmt::print(stderr, "Failed to read {}\n", input_obj);
        exit(1);
    }

    tree.init(V, L, split_limit);
    poly.init(tree, V, L);
    utils::vertex_cell_adj(L, adj);

    {
        auto [left, top, right, bottom] = utils::bbox(V);
        double w = right - left, h = top - bottom;
        sigma *= std::max(w, h);

        left -= w * 0.15;
        right += w * 0.15;
        top += h * 0.15;
        bottom -= h * 0.15;

        utils::generate_grid2d_vertices(
            left, top, right, bottom,
            resolution, resolution, gridV);
        utils::generate_grid2d_cells(resolution, resolution, gridC);
        value.resize(gridV.rows(), 2);
    }

    co_await dispatch_cmd;

    auto out = fmt::output_file(output_vtk);
    io::write_vtk_header(out);
    io::write_vtk_vertex(out, gridV);
    io::write_vtk_cells_homogeneous(out, gridC, 8);
    io::write_vtk_point_data(out, gridV.rows());
    io::write_vtk_vectors(out, value, "gauwn");
}

} // namespace zjucad::gauwn_cli
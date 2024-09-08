module;
#include <string>

#include <CLI/CLI.hpp>
#include <Eigen/Core>
#include <fmt/format.h>
#include <fmt/os.h>

module zjucad.gauwn_cli.veclocgrad;

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
Subcmd cmd_veclocgrad(CLI::App *app, SubcmdContext *ctx) {
    std::string input_obj;
    std::string output_vtk;
    int repeat = 1;
    double sigma = 0.01;
    std::tuple<double, double> tt;

    app->add_option("--input-obj", input_obj, "Input obj file")->required();
    app->add_option("--output-vtk", output_vtk, "Output vtk file")->required();
    app->add_option("--sigma", sigma, "Sigma for Gaussian function")->required();
    app->add_option("--query", tt, "query point")->required();
    app->add_option("--repeat", repeat, "Repeat for timing");

    app->require_subcommand(1);

    bvh2d::AABBTree tree;
    bvh2d::AABBApproxPolygon poly;
    utils::VertexCellAdj adj;
    Eigen::MatrixXd V;
    Eigen::MatrixXi L;
    Eigen::MatrixXd value;

    auto run = [&](auto &&f) {
        double t = utils::time([&] {
            value.setZero();
            Eigen::Vector2d t{std::get<0>(tt), std::get<1>(tt)};
            f(t, 1.0, value);
        },
                               repeat);
        fmt::print("Time: {}\n", t);
    };

    auto gauwn = [&](CLI::App *app, SubcmdContext *ctx) -> Subcmd {
        co_await dispatch_cmd;
        piecewise2d::Gradient grad_eval_pw{.sigma = sigma};

        meshwise2d::VertexLocationGradientAABB<piecewise2d::Gradient> grad_eval_mw_aabb{
            .gradient = grad_eval_pw, .V = &V, .L = &L, .adj = &adj, .tree = &tree, .poly = &poly};

        run(grad_eval_mw_aabb);
        co_await dispatch_cmd;
    };
    ctx->subcmd(app->add_subcommand("gauwn"), gauwn);

    co_await dispatch_cmd;

    if (!zjucad::gauwn::io::read_obj2d(input_obj, V, L)) {
        fmt::print(stderr, "Failed to read {}\n", input_obj);
        exit(1);
    }

    tree.init(V, L, 4);
    poly.init(tree, V, L);
    utils::vertex_cell_adj(L, adj);

    {
        auto [left, top, right, bottom] = utils::bbox(V);
        double w = right - left, h = top - bottom;
        sigma *= std::max(w, h);

        value.resize(V.rows(), 2);
    }

    co_await dispatch_cmd;

    auto out = fmt::output_file(output_vtk);
    io::write_vtk_header(out);
    io::write_vtk_vertex(out, V);
    io::write_vtk_point_data(out, V.rows());
    io::write_vtk_vectors(out, value, "gauwn");
}

} // namespace zjucad::gauwn_cli
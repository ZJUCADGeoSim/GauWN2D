module;
#include <Eigen/Core>
#include <Eigen/Dense>

export module zjucad.gauwn.meshwise2d.aabb;

import zjucad.gauwn.utils.topology;
import zjucad.gauwn.piecewise2d;
import zjucad.gauwn.bvh2d.aabb;
import zjucad.gauwn.bvh2d.approx_polygon;

using namespace Eigen;
namespace zjucad::gauwn::meshwise2d {

export template <piecewise2d::Value PiecewiseValue>
struct ValueAABB {
    PiecewiseValue value;
    const Eigen::MatrixXd *V;
    const Eigen::MatrixXi *L;
    const utils::VertexCellAdj *adj;
    const bvh2d::AABBTree *tree;
    const bvh2d::AABBApproxPolygon *poly;
    double beta = 8.0;

    double operator()(Vector2d t) {
        return operator()(t, 0);
    }

    double operator()(Vector2d t, int node_id) const {
        double wg = 0;
        auto &v = *V;
        auto &l = *L;

        const bvh2d::AABBTree::Node &node = tree->nodes[node_id];
        if (node.far_enough(t, beta)) {
            for (auto l : poly->nodes[node_id]) {
                wg += value(v.row(l(1)), v.row(l(0)), t);
            }
            return wg;
        } else {

            if (node.is_leaf) {
                for (int i : node.ids) {
                    wg += value(v.row(l(i, 1)), v.row(l(i, 0)), t);
                }
            } else {
                for (int i : node.ids) {
                    wg += operator()(t, i);
                }
            }
            return wg;
        }
    }
};
export template <piecewise2d::Gradient PiecewiseGradient>
struct SpatialGradientAABB {
    PiecewiseGradient gradient;
    const Eigen::MatrixXd *V;
    const Eigen::MatrixXi *L;
    const utils::VertexCellAdj *adj;
    const bvh2d::AABBTree *tree;
    const bvh2d::AABBApproxPolygon *poly;
    double beta = 8.0;

    void operator()(Vector2d t, double scale, Vector2d &g) {
        operator()(t, scale, 0, g);
    }

    void operator()(Vector2d t, double scale, int node_id, Vector2d &g) const {
        const auto &node = tree->nodes[node_id];
        if (node.far_enough(t, beta))
            return;
        if (node.is_leaf) {
            auto &v = *V;
            auto &l = *L;

            for (int i : node.ids) {
                {
                    auto &&arp = utils::get_around(v, l, *adj, l(i, 0));
                    auto &&[p_p, p0, p_n] = arp;

                    g -= gradient(p_p, p0, p_n, t) * scale;
                }
                if ((*adj)[l(i, 1)].size() == 1) {
                    auto &&arp = utils::get_around(v, l, *adj, l(i, 1));
                    auto &&[p_p, p0, p_n] = arp;

                    g -= gradient(p_p, p0, p_n, t) * scale;
                }
            }
        } else {
            for (int i : node.ids) {
                operator()(t, scale, i, g);
            }
        }
    }
};

export template <piecewise2d::Gradient PiecewiseGradient>
struct VertexLocationGradientAABB {
    PiecewiseGradient gradient;
    const Eigen::MatrixXd *V;
    const Eigen::MatrixXi *L;
    const utils::VertexCellAdj *adj;
    const bvh2d::AABBTree *tree;
    const bvh2d::AABBApproxPolygon *poly;
    double beta = 8.0;

    void operator()(Vector2d t, double scale, MatrixXd &g) {
        operator()(t, scale, 0, g);
    }

    void operator()(Vector2d t, double scale, int node_id, MatrixXd &g) const {
        const auto &node = tree->nodes[node_id];
        if (node.far_enough(t, beta))
            return;
        if (node.is_leaf) {
            auto &v = *V;
            auto &l = *L;

            for (int i : node.ids) {
                auto &&arp = utils::get_around(v, l, *adj, l(i, 0));
                auto &&[p_p, p0, p_n] = arp;

                g.row(l(i, 0)) += gradient(p_p, p0, p_n, t) * scale;
            }
        } else {
            for (int i : node.ids) {
                operator()(t, scale, i, g);
            }
        }
    }
};

} // namespace zjucad::gauwn::meshwise2d
module;
#include <vector>

#include <absl/container/inlined_vector.h>
#include <Eigen/Core>

export module zjucad.gauwn.bvh2d.approx_polygon;

import zjucad.gauwn.bvh2d.aabb;

using namespace Eigen;

namespace zjucad::gauwn::bvh2d {
export struct AABBApproxPolygon {
    std::vector<
        absl::InlinedVector<Vector2i, 4>>
        nodes;
    void init(const AABBTree &aabb, const MatrixXd &v, const MatrixXi &l);
};

void generate_poly(
    const AABBTree &aabb,
    const AABBTree::Node &node, int current,
    const MatrixXd &v,
    const MatrixXi &l,
    AABBApproxPolygon &poly);

void init_approx_polygon(
    const AABBTree &aabb,
    const MatrixXd &v,
    const MatrixXi &l,
    AABBApproxPolygon &poly) {
    poly.nodes.clear();
    poly.nodes.resize(aabb.nodes.size());
    generate_poly(aabb, aabb.nodes[0], 0, v, l, poly);
}

void AABBApproxPolygon::init(
    const AABBTree &aabb, const MatrixXd &v, const MatrixXi &l) {
    init_approx_polygon(aabb, v, l, *this);
}
} // namespace zjucad::gauwn::bvh2d
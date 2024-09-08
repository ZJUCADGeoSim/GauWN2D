module;

#include <span>
#include <numeric>
#include <ranges>

#include <absl/container/inlined_vector.h>
#include <Eigen/Core>

export module zjucad.gauwn.bvh2d.aabb;

using namespace Eigen;

namespace zjucad::gauwn::bvh2d {

// left, right, bottom, top, cx, cy
export using BBox = std::array<double, 6>;

constexpr std::size_t K = 4;

BBox compute_bbox(
    auto &&bboxes) {
    constexpr double inf = std::numeric_limits<double>::infinity();
    BBox ret{inf, -inf, inf, -inf, 0, 0};
    for (const auto &b : bboxes) {
        ret[0] = std::min(ret[0], b[0]);
        ret[1] = std::max(ret[1], b[1]);

        ret[2] = std::min(ret[2], b[2]);
        ret[3] = std::max(ret[3], b[3]);
    }
    ret[4] = (ret[0] + ret[1]) / 2.0;
    ret[5] = (ret[2] + ret[3]) / 2.0;
    return ret;
}

double squared_radius_of_bbox(const BBox &bbox) {
    Vector2d center{bbox[4], bbox[5]}, corner{bbox[0], bbox[2]};
    return (center - corner).squaredNorm();
}
struct AABBNode {
    BBox bbox;
    double squared_radius;
    double radius;
    double sqrt_radius;

    absl::InlinedVector<std::size_t, K> ids;
    bool is_leaf = false;

    [[nodiscard]] double squared_distance(
        Eigen::Vector2d point) const {
        return (point - Vector2d{bbox[4], bbox[5]}).squaredNorm();
    }

    [[nodiscard]] bool far_enough(Vector2d p, double beta) const {
        return squared_distance(p) > beta * beta * squared_radius;
    }
};

export struct AABBTree {

    using Node = AABBNode;

    std::vector<Node> nodes;
    std::size_t split_limit;

    std::array<std::span<std::size_t>, K> split(
        std::span<std::size_t> primitive_ids,
        std::span<const BBox> primitive_bbox,
        const BBox &current_bbox) {
        double xspan = current_bbox[1] - current_bbox[0];
        double yspan = current_bbox[3] - current_bbox[2];

        std::size_t axis = xspan < yspan ? 1 : 0;
        std::ranges::sort(
            primitive_ids, [&](std::size_t l, std::size_t r) {
                return primitive_bbox[l][4 + axis] < primitive_bbox[r][4 + axis];
            });
        std::array<std::span<std::size_t>, K> ret;
        std::size_t n = primitive_ids.size() / K;
        for (int i = 0; i < K - 1; i++) {
            ret[i] = std::span{primitive_ids.data() + i * n, n};
        }
        ret[K - 1] = std::span{
            primitive_ids.data() + (K - 1) * n, primitive_ids.size() - (K - 1) * n};

        return ret;
    }
    void construct(
        std::span<std::size_t> primitive_ids,
        int node_id,
        std::span<const BBox> primitive_bbox) {
        if (primitive_ids.size() < split_limit) {
            nodes[node_id].is_leaf = true;
            nodes[node_id].ids.resize(primitive_ids.size());
            std::ranges::copy(primitive_ids, nodes[node_id].ids.begin());
        } else {
            auto sp = split(primitive_ids, primitive_bbox, nodes[node_id].bbox);
            nodes[node_id].ids.reserve(nodes[node_id].ids.size() + sp.size());
            for (auto child_ps : sp) {
                int child_id = nodes.size();
                nodes[node_id].ids.emplace_back(child_id);

                nodes.emplace_back(Node{
                    .bbox = compute_bbox(
                        child_ps | std::views::transform([&](int id) { return primitive_bbox[id]; }))});
                construct(child_ps, child_id, primitive_bbox);
            }
        }
        nodes[node_id].squared_radius = squared_radius_of_bbox(nodes[node_id].bbox);
        nodes[node_id].radius = std::sqrt(nodes[node_id].squared_radius);
        nodes[node_id].sqrt_radius = std::sqrt(nodes[node_id].radius);
    }

    void init(
        const MatrixXd &V,
        const MatrixXi &T,
        std::size_t split_limit) {
        this->split_limit = split_limit;

        std::vector<BBox> primitive_bbox;
        primitive_bbox.clear();
        nodes.clear();
        primitive_bbox.reserve(T.size());
        constexpr double inf = std::numeric_limits<double>::infinity();

        for (int i = 0; i < T.rows(); i++) {
            primitive_bbox.push_back(
                {inf, -inf, inf, -inf, 0, 0});
            BBox &b = primitive_bbox.back();
            for (int k = 0; k < 2; k++) {
                for (int j = 0; j < V.cols(); j++) {
                    b[j * 2] = std::min(b[j * 2], V(T(i, k), j));
                    b[j * 2 + 1] = std::max(b[j * 2 + 1], V(T(i, k), j));
                }
            }
            b[4] = (b[0] + b[1]) / 2.0;
            b[5] = (b[2] + b[3]) / 2.0;
        }
        nodes.emplace_back(
            Node{
                .bbox{compute_bbox(primitive_bbox)},
            });
        std::vector<std::size_t> ps(T.rows());
        std::iota(ps.begin(), ps.end(), 0);

        construct(ps, 0, primitive_bbox);
    }
};

} // namespace zjucad::gauwn::bvh2d
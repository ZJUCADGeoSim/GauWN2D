module;
#include <Eigen/Core>
#include <unordered_map>
#include <unordered_set>

module zjucad.gauwn.bvh2d.approx_polygon;

import zjucad.gauwn.bvh2d.aabb;
import zjucad.gauwn.utils.union_find;

using namespace Eigen;

namespace zjucad::gauwn::bvh2d {

void generate_poly(
    const AABBTree &aabb,
    const AABBTree::Node &node, int current,
    const MatrixXd &v,
    const MatrixXi &l,
    AABBApproxPolygon &poly) {

    if (node.is_leaf) {
        std::unordered_map<int, int> parent;
        std::unordered_set<int> left, right;

        for (auto id : node.ids) {
            int la = l(id, 0), lb = l(id, 1);
            parent[la] = la;
            parent[lb] = lb;
            left.insert(la);
            right.insert(lb);
        }
        for (auto id : node.ids) {
            int la = l(id, 0), lb = l(id, 1);
            utils::uadd(la, lb, parent);
        }
        for (auto kl : left) {
            for (auto kr : right) {
                if (!right.contains(kl) && !left.contains(kr) &&
                    utils::ufind(kl, parent) == utils::ufind(kr, parent)) {
                    poly.nodes[current].emplace_back(kl, kr);
                }
            }
        }

    } else {
        for (int id : node.ids) {
            generate_poly(aabb, aabb.nodes[id], id, v, l, poly);
        }
        std::unordered_map<int, int> parent;
        std::unordered_set<int> left, right;

        for (int id : node.ids) {
            for (auto &&j : poly.nodes[id]) {
                int la = j[0], lb = j[1];
                parent[la] = la;
                parent[lb] = lb;
                left.insert(la);
                right.insert(lb);
            }
        }

        for (int id : node.ids) {
            for (auto &&j : poly.nodes[id]) {
                int la = j[0], lb = j[1];
                utils::uadd(la, lb, parent);
            }
        }
        for (auto kl : left) {
            for (auto kr : right) {
                if (!right.contains(kl) && !left.contains(kr) &&
                    utils::ufind(kl, parent) == utils::ufind(kr, parent)) {
                    poly.nodes[current].emplace_back(kl, kr);
                }
            }
        }
    }
}
} // namespace zjucad::gauwn::bvh2d
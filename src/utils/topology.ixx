module;

#include <vector>

#include <absl/container/inlined_vector.h>
#include <Eigen/Core>

export module zjucad.gauwn.utils.topology;

using namespace Eigen;

namespace zjucad::gauwn::utils {
export using VertexCellAdj = std::vector<absl::InlinedVector<int, 4>>;

export void vertex_cell_adj(
    const MatrixXi &L,
    VertexCellAdj &adj) {
    adj.resize(L.maxCoeff() + 1);
    for (int i = 0; i < L.rows(); i++) {
        for (int j = 0; j < L.cols(); j++) {
            adj[L(i, j)].push_back(i);
        }
    }
}
export std::array<Vector2d, 3> get_around(
    const MatrixXd &V,
    const MatrixXi &L,
    const VertexCellAdj &adj,
    int p) {
    if (adj[p].size() == 2) {
        int p0 = p,
            p_n = L(adj[p][0], 0) ^ L(adj[p][0], 1) ^ p,
            p_p = L(adj[p][1], 0) ^ L(adj[p][1], 1) ^ p;
        if (L(adj[p][1], 0) == p) {
            std::swap(p_n, p_p);
        }
        return {V.row(p_p), V.row(p0), V.row(p_n)};
    } else {
        int p_other = L(adj[p][0], 0) ^ L(adj[p][0], 1) ^ p;
        if (L(adj[p][0], 0) == p) {
            return {V.row(p), V.row(p), V.row(p_other)};
        } else {
            return {V.row(p_other), V.row(p), V.row(p)};
        }
    }
}

} // namespace zjucad::gauwn::utils
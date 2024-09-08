module;
#include <Eigen/Core>

export module zjucad.gauwn.utils.grid2d;

namespace zjucad::gauwn::utils {
export void generate_grid2d_vertices(
    double left, double top, double right, double bottom, int rx, int ry,
    Eigen::MatrixXd &V) {
    V.resize(rx * ry, 2);
    double dx = (right - left) / rx, dy = (top - bottom) / ry;
    for (int i = 0; i < ry; i++) {
        for (int j = 0; j < rx; j++)
            V.row(i * rx + j) = Eigen::Vector2d{left + j * dx, bottom + i * dy};
    }
}
export void generate_grid2d_cells(int rx, int ry, Eigen::MatrixXi &C) {
    C.resize((rx - 1) * (ry - 1), 4);
    for (int i = 0; i < ry - 1; i++) {
        for (int j = 0; j < rx - 1; j++) {
            C.row(i * (rx - 1) + j) = Eigen::Vector4i{
                i * rx + j, i * rx + rx + j, i * rx + j + 1, i * rx + rx + j + 1};
        }
    }
}
} // namespace zjucad::gauwn::utils
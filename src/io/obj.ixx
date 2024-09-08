module;
#include <Eigen/Core>

export module zjucad.gauwn.io.obj;

namespace zjucad::gauwn::io {
export bool read_obj2d(const std::string &filepath, Eigen::MatrixXd &V, Eigen::MatrixXi &L);
export bool read_obj3d(const std::string &filepath, Eigen::MatrixXd &V, Eigen::MatrixXi &F);
} // namespace zjucad::gauwn::io
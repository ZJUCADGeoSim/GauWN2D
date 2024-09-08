module;
#include <Eigen/Core>
#include <fmt/os.h>

export module zjucad.gauwn.io.vtk;
import stdgenerator;

namespace zjucad::gauwn::io {

export void write_vtk_header(fmt::ostream &out);
export void write_vtk_vertex(fmt::ostream &out, const Eigen::MatrixXd &vertex);
export void write_vtk_cells_heterogeneous(
    fmt::ostream &out, const std::generator<int> &info);
export void write_vtk_cells_homogeneous(
    fmt::ostream &out, const Eigen::MatrixXi &cells, int cell_type);

export void write_vtk_point_data(
    fmt::ostream &out, Eigen::Index n);
export void write_vtk_cell_data(
    fmt::ostream &out, int n);
export void write_vtk_scalars(
    fmt::ostream &out, const Eigen::VectorXd &data, const std::string &name);

export void write_vtk_vectors(
    fmt::ostream &out, const Eigen::MatrixXd &data, const std::string &name);

export void write_vtk_field(
    fmt::ostream &out, const Eigen::MatrixXd &data, const std::string &name);
export void write_vtk_curve(fmt::ostream &out, const Eigen::MatrixXd &vertex, const Eigen::MatrixXi &line);

} // namespace zjucad::gauwn::io

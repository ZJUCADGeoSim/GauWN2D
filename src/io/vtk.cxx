module;
#include <Eigen/Core>
#include <fmt/format.h>
#include <fmt/os.h>

module zjucad.gauwn.io.vtk;

namespace zjucad::gauwn::io {

void write_vtk_header(fmt::ostream &out) {
    out.print("# vtk DataFile Version 3.0\n");
    out.print("vtk output\n");
    out.print("ASCII\n");
    out.print("DATASET UNSTRUCTURED_GRID\n");
}

void write_vtk_vertex(fmt::ostream &out, const Eigen::MatrixXd &vertex) {
    out.print("POINTS {} double\n", vertex.rows());
    if (vertex.cols() == 2) {
        for (int i = 0; i < vertex.rows(); i++) {
            out.print("{} {} 0\n", vertex(i, 0), vertex(i, 1));
        }
    } else if (vertex.cols() == 3) {
        for (int i = 0; i < vertex.rows(); i++) {
            out.print("{} {} {}\n", vertex(i, 0), vertex(i, 1), vertex(i, 2));
        }
    } else {
        throw std::runtime_error("unsupported vertex dimension");
    }
}

void write_vtk_cells_homogeneous(
    fmt::ostream &out, const Eigen::MatrixXi &cells, int cell_type) {
    out.print("CELLS {} {}\n", cells.rows(), cells.rows() + cells.size());
    for (int i = 0; i < cells.rows(); i++) {
        out.print("{} ", cells.cols());
        for (int j = 0; j < cells.cols(); j++) {
            out.print("{} ", cells(i, j));
        }
        out.print("\n");
    }
    out.print("CELL_TYPES {}\n", cells.rows());
    for (int i = 0; i < cells.rows(); i++) {
        out.print("{}\n", cell_type);
    }
}
void write_vtk_cells_heterogeneous(
    fmt::ostream &out, const std::generator<int> &info) {
    auto it = info.begin();
    int n_cells = *it;
    it++;
    int n_all_verts = *it;
    it++;
    out.print("CELLS {} {}\n", n_cells, n_all_verts + n_cells);
    for (int i = 0; i < n_cells; i++) {
        int n = *it;
        it++;
        out.print("{} ", n);
        for (int j = 0; j < n; j++) {
            out.print("{} ", *it);
            it++;
        }
        out.print("\n");
    }
    out.print("CELL_TYPES {}\n", n_cells);
    for (int i = 0; i < n_cells; i++) {
        out.print("{}\n", *it);
        it++;
    }
}
void write_vtk_point_data(fmt::ostream &out, Eigen::Index n) {
    out.print("POINT_DATA {}\n", n);
}

void write_vtk_cell_data(fmt::ostream &out, int n) {
    out.print("CELL_DATA {}\n", n);
}

void write_vtk_scalars(
    fmt::ostream &out, const Eigen::VectorXd &data, const std::string &name) {
    out.print("SCALARS {} double 1\n", name);
    out.print("LOOKUP_TABLE default\n", name);
    for (int i = 0; i < data.rows(); i++) {
        out.print("{}\n", data(i));
    }
}

void write_vtk_vectors(
    fmt::ostream &out, const Eigen::MatrixXd &data, const std::string &name) {
    out.print("VECTORS {} double\n", name);
    if (data.cols() == 2) {
        for (int i = 0; i < data.rows(); i++) {
            out.print("{} {} 0\n", data(i, 0), data(i, 1));
        }
    } else if (data.cols() == 3) {
        for (int i = 0; i < data.rows(); i++) {
            out.print("{} {} {}\n", data(i, 0), data(i, 1), data(i, 2));
        }
    } else {
        throw std::runtime_error("unsupported point data dimension");
    }
}

void write_vtk_cell_data_scalars(
    fmt::ostream &out, const Eigen::VectorXd &cell_data, const std::string &name) {
    out.print("SCALARS {} double 1\n", name);
    out.print("LOOKUP_TABLE default\n", name);
    for (int i = 0; i < cell_data.rows(); i++) {
        out.print("{}\n", cell_data(i));
    }
}

void write_vtk_field(
    fmt::ostream &out, const Eigen::MatrixXd &data, const std::string &name) {
    out.print("FIELD {} 1\n", name);
    out.print("{} {} {} double\n", name, data.cols(), data.rows());
    for (int i = 0; i < data.rows(); i++) {
        for (int j = 0; j < data.cols(); j++) {
            out.print("{} ", data(i, j));
        }
        out.print("\n");
    }
}

void write_vtk_curve(
    fmt::ostream &out, const Eigen::MatrixXd &vertex, const Eigen::MatrixXi &line) {
    write_vtk_header(out);
    write_vtk_vertex(out, vertex);
    write_vtk_cells_homogeneous(out, line, 3);
}

} // namespace zjucad::gauwn::io

module;
#include <Eigen/Core>
#include <fstream>

module zjucad.gauwn.io.obj;

namespace zjucad::gauwn::io {
bool read_obj2d(const std::string &filepath, Eigen::MatrixXd &V, Eigen::MatrixXi &L) {
    std::ifstream in{filepath};
    bool read_error = false;
    if (!in) {
        read_error = true;
        return !read_error;
    }
    std::vector<Eigen::Vector2d> vertex;
    std::vector<Eigen::Vector2i> line;
    char c = ' ';
    double n1 = 0, n2 = 0;
    while (in >> c >> n1 >> n2) {
        if (c == 'v') {
            vertex.emplace_back(n1, n2);
        } else if (c == 'l') {
            line.emplace_back(n1 - 1, n2 - 1);
        }
    }
    V.resize(int(vertex.size()), 2);
    for (int i = 0; i < vertex.size(); i++)
        V.row(i) = vertex[i];
    L.resize(int(line.size()), 2);
    for (int i = 0; i < line.size(); i++)
        L.row(i) = line[i];
    return !read_error;
}

} // namespace zjucad::gauwn::io
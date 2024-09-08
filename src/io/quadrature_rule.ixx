module;
#include <Eigen/Core>

#include <iostream>
#include <fstream>
#include <vector>

export module zjucad.gauwn.io.quadrature_rule;

namespace zjucad::gauwn::io {
export std::vector<Eigen::MatrixXd> read_quadrature_rule(const std::string &path) {
    std::vector<Eigen::MatrixXd> quadrature_rule;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return quadrature_rule;
    }
    int num_rules;
    file >> num_rules;
    quadrature_rule.resize(num_rules);
    for (int i = 0; i < num_rules; i++) {
        int num_points;
        file >> num_points;
        quadrature_rule[i].resize(num_points, 4);
        for (int j = 0; j < num_points; j++) {
            file >> quadrature_rule[i](j, 0) >> quadrature_rule[i](j, 1) >> quadrature_rule[i](j, 2) >> quadrature_rule[i](j, 3);
        }
    }
    return quadrature_rule;
}
} // namespace zjucad::gauwn::io
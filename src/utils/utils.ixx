module;
#include <Eigen/Core>
#include <chrono>
export module zjucad.gauwn.utils;

namespace zjucad::gauwn::utils {
export std::tuple<double, double, double, double> bbox(
    const Eigen::MatrixXd &V) {
    double left = V.col(0).minCoeff(),
           right = V.col(0).maxCoeff(),
           bottom = V.col(1).minCoeff(),
           top = V.col(1).maxCoeff();
    return {left, top, right, bottom};
}

// time unit: second
export double time(auto &&f, int repeat = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < repeat; i++)
        f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count() / repeat;
}
} // namespace zjucad::gauwn::utils
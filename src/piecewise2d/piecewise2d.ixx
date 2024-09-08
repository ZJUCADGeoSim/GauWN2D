module;
#include <Eigen/Core>

export module zjucad.gauwn.piecewise2d;

namespace zjucad::gauwn::piecewise2d {

export template <typename T>
concept Value = requires(T v, Eigen::Vector2d v0, Eigen::Vector2d v1, Eigen::Vector2d t) {
    { v(v0, v1, t) } -> std::convertible_to<double>;
};

export template <typename T>
concept Gradient = requires(
    T v,
    Eigen::Vector2d vp, Eigen::Vector2d vi, Eigen::Vector2d vn, Eigen::Vector2d t) {
    { v(vp, vi, vn, t) } -> std::convertible_to<Eigen::Vector2d>;
};
} // namespace zjucad::gauwn::piecewise2d
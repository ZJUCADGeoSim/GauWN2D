module;
#include <Eigen/Core>

module zjucad.gauwn.piecewise2d.value;

import zjucad.gauwn.numerics.gaussian_integrals;

namespace zjucad::gauwn::piecewise2d {
double GauValue::operator()(Eigen::Vector2d vj, Eigen::Vector2d vi, Eigen::Vector2d t) const {
    double xi = vi(0), xj = vj(0), yi = vi(1), yj = vj(1);

    if (std::abs(yj - yi) < 1e-16) [[unlikely]]
        return 0;
    return (yj - yi) * numerics::int_gPhi(0, 1, yi - yj, yj, xi - xj, xj, sigma, t(1), t(0));
}
} // namespace zjucad::gauwn::piecewise2d
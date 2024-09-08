module;
#include <Eigen/Core>
module zjucad.gauwn.piecewise2d.gradient;

import zjucad.gauwn.numerics.gaussian_integrals;
import zjucad.gauwn.numerics.gaussian;
using namespace Eigen;

namespace zjucad::gauwn::piecewise2d {

Vector2d oneside(Vector2d vj, Vector2d vi, double sigma, Vector2d t) {
    Vector2d vec = vi - vj;
    double len = vec.norm();
    if (len < 1e-16) {
        return Vector2d::Zero();
    }
    Vector2d normal{vec(1), -vec(0)};
    normal.normalize();

    double mu = (t - vj).dot(vec) / len;
    double sqrD = (t - vj).squaredNorm() - mu * mu;
    if (sqrD < 0) [[unlikely]] {
        sqrD = 0;
    }
    double d = std::sqrt(sqrD);

    return -numerics::gaussian(sigma, d) * numerics::int_tg(0, len, mu, sigma) / len * normal;
}

Vector2d Gradient::operator()(Vector2d vp, Vector2d vi, Vector2d vn, Vector2d t) const {
    return oneside(vn, vi, sigma, t) - oneside(vp, vi, sigma, t);
}

} // namespace zjucad::gauwn::piecewise2d
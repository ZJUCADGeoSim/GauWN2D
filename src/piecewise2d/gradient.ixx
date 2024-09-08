module;
#include <Eigen/Core>

export module zjucad.gauwn.piecewise2d.gradient;

using namespace Eigen;

namespace zjucad::gauwn::piecewise2d {
export struct Gradient {
    Vector2d operator()(Vector2d vp, Vector2d vi, Vector2d vn, Vector2d t) const;

    double sigma;
};
} // namespace zjucad::gauwn::piecewise2d
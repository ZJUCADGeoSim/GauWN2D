module;
#include <Eigen/Core>

export module zjucad.gauwn.piecewise2d.value;

using namespace Eigen;

namespace zjucad::gauwn::piecewise2d {
export struct GauValue {
    double operator()(Vector2d vj, Vector2d vi, Vector2d t) const;

    double sigma;
};
} // namespace zjucad::gauwn::piecewise2d
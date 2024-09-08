module;
#include <cmath>
#include <Eigen/Core>
#include <fmt/format.h>
#include <fmt/os.h>

export module zjucad.gauwn.numerics.gaussian_integrals;
import zjucad.gauwn.numerics.gaussian;

namespace zjucad::gauwn::numerics {
// `\int G(a+bx)g(x) dx
export double owen10010_1(double Y, double a, double b) {
    double s1bb = std::sqrt(1 + b * b);
    return bvn(a / s1bb, Y, -b / s1bb);
}

// `\int G(a+bx;sigma^2)g(x;sigma^2) dx
export double owen10010_1(double Y, double a, double b, double sigma, double mu_1, double mu_2) {
    return owen10010_1((Y - mu_1) / sigma, (a + mu_1 * b - mu_2) / sigma, b);
}

// Owen1980 10,010.4
// `\int_h^k g(x)\Phi(a+bx) dx`
export double owen10010_4(double h, double k, double a, double b, double sigma, double mu_1, double mu_2) {
    return owen10010_1(k, a, b, sigma, mu_1, mu_2) - owen10010_1(h, a, b, sigma, mu_1, mu_2);
}

// `\int_{t_0}^{t_1} g(a t+b)\Phi(e t + f) dt`
export double int_gPhi(double t0, double t1, double a, double b, double e, double f,
                       double sigma, double mu_1, double mu_2) {
    double aa = f - b * e / a;
    double bb = e / a;
    return owen10010_4(a * t0 + b, a * t1 + b, aa, bb, sigma, mu_1, mu_2) / a;
}

// `int t g(t-mu;sigma^2) dt`
export double int_tg(double t0, double t1, double mu, double sigma) {
    return sigma * sigma * (gaussian(sigma, t0 - mu) - gaussian(sigma, t1 - mu)) +
           mu * (phi(sigma, t1 - mu) - phi(sigma, t0 - mu));
}

} // namespace zjucad::gauwn::numerics

// Copyright 2022 David Cortes

/*
    This file is adapted from https://github.com/david-cortes/approxcdf/blob/master/src/other.cpp
    which is licensed under the BSD 3-Clause License.
*/

module;
#include <cmath>
#include <algorithm>
module zjucad.gauwn.numerics.gaussian;

namespace zjucad::gauwn::numerics {
double norm_pdf_1d(double x) {
    const double inv_sqrt2_pi = 1. / std::sqrt(double(2. * M_PI));

    if (std::isinf(x))
        return 0.;
    if (x < 5. && x > -5.) {
        return inv_sqrt2_pi * std::exp(double(-0.5) * x * x);
    } else [[unlikely]] {
        double x1 = std::floor(x * 0x1.0p16 + 0.5) * 0x1.0p-16;
        double x2 = x - x1;
        return inv_sqrt2_pi * (std::exp(-0.5 * x1 * x1) * std::exp((-0.5 * x2 - x1) * x2));
    }
}

/* Adapted from cephes' 'ndtr' */
double norm_cdf_1d(double x) {
    const double inv_sqrt2 = 1. / std::sqrt(double(2.));
    const double sqrt2 = std::sqrt(double(2.));
    /* This is technically correct but very imprecise:
    return 0.5 * std::erfc(neg_inv_sqrt2 * x);
    */
    // if (std::isinf(x)) {
    //     return (x >= 0.)? 1. : 0.;
    // }
    double x_, y, z;
    x_ = x * inv_sqrt2;
    z = std::fabs(x_);

    /* if( z < SQRTH ) */
    // if (z < 1.) {
    if (z < inv_sqrt2) {
        y = .5 + .5 * std::erf(x_);
    } else {
        y = .5 * std::erfc(z);
        if (x_ > 0.) {
            y = 1. - y;
        }
    }
    return y;
}

/* Tsay, Wen-Jen, and Peng-Hsuan Ke.
   "A simple approximation for the bivariate normal integral."
   Communications in Statistics-Simulation and Computation (2021): 1-14. */
constexpr double c1 = -1.0950081470333;
constexpr double c2 = -0.75651138383854;

double bvn(double x1, double x2, double rho) {
    const double inv_sqrt2 = 1. / std::sqrt(2.);
    const double sqrt2 = std::sqrt(2.);
    if (std::fabs(rho) <= std::numeric_limits<double>::epsilon()) {
        return norm_cdf_1d(x1) * norm_cdf_1d(x2);
    }
    rho = std::clamp(rho, -1 + 1e-8, 1 - 1e-8);

    double denom = std::sqrt(1 - rho * rho);
    double a = -rho / denom;
    double b = x1 / denom;
    double aq_plus_b = a * x2 + b;

    if (a > 0) {
        if (aq_plus_b >= 0) {
            double aa = a * a;
            double a_sq_c1 = aa * c1;
            double a_sq_c2 = aa * c2;
            double sqrt2b = sqrt2 * b;
            double sqrt2x2 = sqrt2 * x2;
            double sqrt_recpr_a_sq_c2 = std::sqrt(1. - a_sq_c2);
            double twicea_sqrt_recpr_a_sq_c2 = 2. * a * sqrt_recpr_a_sq_c2;
            double temp = 1. / (4. * sqrt_recpr_a_sq_c2);
            double t1 = a_sq_c1 * c1 + 2. * b * b * c2;
            double t2 = 2. * sqrt2b * c1;
            double t3 = 4. - 4. * a_sq_c2;

            return 0.5 * (std::erf(x2 / sqrt2) + std::erf(b / (sqrt2 * a))) +
                   temp * std::exp((t1 - t2) / t3) * (1. - std::erf((sqrt2b - a_sq_c1) / twicea_sqrt_recpr_a_sq_c2)) -
                   temp * std::exp((t1 + t2) / t3) * (std::erf((sqrt2x2 - sqrt2x2 * a_sq_c2 - sqrt2b * a * c2 - a * c1) / (2. * sqrt_recpr_a_sq_c2)) + std::erf((a_sq_c1 + sqrt2b) / twicea_sqrt_recpr_a_sq_c2));

        } else {
            double sqrt2b = sqrt2 * b;
            double sqrt2x2 = sqrt2 * x2;
            double a_sq_c2 = a * a * c2;
            double recpr_a_sq_c2 = 1. - a_sq_c2;
            double sqrt_recpr_a_sq_c2 = std::sqrt(recpr_a_sq_c2);
            double a_c1 = a * c1;

            return (1. / (4. * sqrt_recpr_a_sq_c2)) *
                   std::exp((a_c1 * a_c1 - 2. * sqrt2b * c1 + 2 * b * b * c2) / (4. * recpr_a_sq_c2)) *
                   (1. + std::erf((sqrt2x2 - sqrt2x2 * a_sq_c2 - sqrt2b * a * c2 + a_c1) / (2. * sqrt_recpr_a_sq_c2)));
        }
    } else {
        if (aq_plus_b >= 0) {
            double sqrt2b = sqrt2 * b;
            double a_sq_c2 = a * a * c2;
            double recpr_a_sq_c2 = 1. - a_sq_c2;
            double sqrt_recpr_a_sq_c2 = std::sqrt(recpr_a_sq_c2);
            double a_c1 = a * c1;
            double sqrt2_x2 = sqrt2 * x2;

            return 0.5 + 0.5 * std::erf(x2 / sqrt2) -
                   (1. / (4. * sqrt_recpr_a_sq_c2)) *
                       std::exp((a_c1 * a_c1 + 2. * sqrt2b * c1 + 2. * b * b * c2) / (4. * recpr_a_sq_c2)) *
                       (1. + std::erf((sqrt2_x2 - sqrt2_x2 * a_sq_c2 - sqrt2b * a * c2 - a_c1) / (2. * sqrt_recpr_a_sq_c2)));
        } else {
            double sqrt2a = sqrt2 * a;
            double sqrt2b = sqrt2 * b;
            double a_sq_c2 = a * a * c2;
            double recpr_a_sq_c2 = 1. - a_sq_c2;
            double sqrt_recpr_a_sq_c2 = std::sqrt(recpr_a_sq_c2);
            double a_c1 = a * c1;
            double temp = 1. / (4. * sqrt_recpr_a_sq_c2);
            double t1 = a_c1 * a_c1 + 2. * b * b * c2;
            double t2 = 2. * sqrt2b * c1;
            double t3 = 4. * recpr_a_sq_c2;
            double sqrt2_x2 = sqrt2 * x2;

            return 0.5 - 0.5 * std::erf(b / sqrt2a) -
                   temp * std::exp((t1 + t2) / t3) * (1. - std::erf((sqrt2b + a * a_c1) / (2. * a * sqrt_recpr_a_sq_c2))) +
                   temp * std::exp((t1 - t2) / t3) * (std::erf((sqrt2_x2 - sqrt2_x2 * a_sq_c2 - sqrt2b * a * c2 + a_c1) / (2. * sqrt_recpr_a_sq_c2)) + std::erf((sqrt2b - a * a_c1) / (2. * a * sqrt_recpr_a_sq_c2)));
        }
    }
}

} // namespace zjucad::gauwn::numerics
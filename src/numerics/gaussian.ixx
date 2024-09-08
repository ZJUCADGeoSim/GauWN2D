module;
export module zjucad.gauwn.numerics.gaussian;

namespace zjucad::gauwn::numerics {
export double norm_pdf_1d(double x);
export double norm_cdf_1d(double x);
export double norm_lcdf_1d(double x) {
    return norm_cdf_1d(-x);
}
export double bvn(double x, double y, double rho);

export double gaussian(double sigma, double x) {
    return norm_pdf_1d(x / sigma) / sigma;
}
export double phi(double sigma, double x) {
    return norm_cdf_1d(x / sigma);
}
} // namespace zjucad::gauwn::numerics
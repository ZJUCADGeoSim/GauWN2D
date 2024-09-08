target_sources(GauWN PUBLIC
    FILE_SET cxx_modules_piecewise2d TYPE CXX_MODULES FILES

    piecewise2d.ixx
    value.ixx
    gradient.ixx
)

target_sources(GauWN PUBLIC value.cxx gradient.cxx)
target_sources(GauWN PUBLIC
    FILE_SET cxx_modules_io TYPE CXX_MODULES FILES

    vtk.ixx
    obj.ixx
    quadrature_rule.ixx
)

target_sources(GauWN PUBLIC obj.cxx vtk.cxx)
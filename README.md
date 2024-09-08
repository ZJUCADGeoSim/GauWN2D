# GauWN: Gaussian-smoothed Winding Number and its Derivatives

```bash
build/bin/gauwn-cli value --input-obj data/bunny_abstract.obj --output-vtk value.vtk --sigma 0.02 --resolution 200 gauwn
build/bin/gauwn-cli spgrad --input-obj data/bunny_abstract.obj --output-vtk spgrad.vtk --sigma 0.02 --resolution 200 gauwn
build/bin/gauwn-cli veclocgrad --input-obj data/bunny_abstract.obj --output-vtk veclocgrad.vtk --sigma 0.02 --query 0.0856 0.112 gauwn
```
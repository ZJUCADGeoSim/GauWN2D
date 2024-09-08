# GauWN: Gaussian-smoothed Winding Number and its Derivatives

## Prequeites and Dependenciess
* Clang++, libcxx, lld 18.1.8
* CMake 3.29.0 and above
* Eigen 3.4
* Abseil 2024.01.16
* fmtlib 10.2
* CLI11

## Build
* Option 1: build with Vcpkg (recommended)
```bash
cmake -S . -B build --preset release
cmake --build build
```
* Option 2: manually install the dependencies
```bash
cmake -S . -B build --preset release-m
cmake --build build-m
```

## Usage example
You can find examples for this library in `cli/` directory, and you can run the example in the following way:

```bash
build/bin/gauwn-cli value --input-obj data/bunny_abstract.obj --output-vtk value.vtk --sigma 0.02 --resolution 200 gauwn
build/bin/gauwn-cli spgrad --input-obj data/bunny_abstract.obj --output-vtk spgrad.vtk --sigma 0.02 --resolution 200 gauwn
build/bin/gauwn-cli veclocgrad --input-obj data/bunny_abstract.obj --output-vtk veclocgrad.vtk --sigma 0.02 --query 0.0856 0.112 gauwn
```


## BibTeX
```bibtex
@inproceedings{10.1145/3680528.3687569,
    author = {Sun, Haoran and Wang, Jingkai and Bao, Hujun and Huang, Jin},
    title = {GauWN: Gaussian-smoothed Winding Number and its Derivatives},
    year = {2024},
    isbn = {9798400711312},
    publisher = {Association for Computing Machinery},
    address = {New York, NY, USA},
    url = {https://doi.org/10.1145/3680528.3687569},
    doi = {10.1145/3680528.3687569},
    booktitle = {SIGGRAPH Asia 2024 Conference Papers},
    articleno = {12},
    numpages = {9},
    keywords = {Winding number, geometry processing, differentiable},
    location = {Tokyo, Japan},
    series = {SA '24}
}
```
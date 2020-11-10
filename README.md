# Particle Identification Framework

## Installation

    cd pid
    mkdir build install
    cd build
    source /path-to-root/thisroot.sh
    cmake -DCMAKE_INSTALL_PREFIX=../install ../
    make -j install

## Examples and short description 

### Fitting
To modify fitting algorithm, functions, etc change file macro/fit_botvina12.C (follow documentation inside)

    cd build
    make
    cd ../macro
    root fit_botvina12.C

### Getter
Example how to use output file:

    cd macro
    root -l RunGetter.C

### Doxygen documentation
    doxygen Doxyfile
File Doxygen/html/index.html with documentation will be created

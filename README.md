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
To modify fitting algorithm, functions, etc change file main.cpp (follow documentation inside)

    cd build
    make
    ./main

### Getter
Example how to use output file:

    cd macro
    root -l RunGetter.C

### Doxygen documentation
    doxygen Doxyfile
File Doxygen/html/index.html with documentation will be created
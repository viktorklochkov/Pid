# Particle Identification Framework

## Installation

    cd pid
    mkdir build
    cd build
    source /path-to-root/thisroot.sh
    cmake ../
    make

## Examples and short description 

### Fitting
To modify fitting algorithm and function change file main.cpp

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
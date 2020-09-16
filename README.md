# Numeric library
This library contains a bunch of classes and functions to do various math things.

## Contents
### Classes
  - Matrix
  - Vector
  - Benchmark
  
### Functions
  - RREF
  - Gauss jordan elimination

## How to build
To build and test this library, do the following:
  - clone: `git clone https://github.com/amartya00/numeric.git`
  - `cd numeric`
  - `mkdir _build && cd _build`
  - `meson ..`
  - `ninja test`
  
To build documentation:
  - clone: `git clone https://github.com/amartya00/numeric.git`
  - `cd numeric`
  - `doxygen .doxyfile`

This will generate documentation in html and latex formats.
  

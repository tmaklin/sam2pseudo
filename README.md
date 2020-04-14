# longsweep
Converting sam files and long-read alignments to pseudoalignments.

## Installation
### Compiling from source
#### Requirements
- C++11 compliant compiler
- cmake v2.8.12 or newer

#### Compilation
Clone the repository
```
git clone https://github.com/tmaklin/longsweep.git
```
enter the directory and run
```
mkdir build
cd build
cmake ..
make
```

## Usage
Type
```
sam2pseudo alignment.sam
```
which will print the corresponding Themisto pseudoalignment format to
cout.

## License
The source code from this project is subject to the terms of the MIT
license. A copy of the MIT license is supplied with the project, or
can be obtained at https://opensource.org/licenses/MIT.

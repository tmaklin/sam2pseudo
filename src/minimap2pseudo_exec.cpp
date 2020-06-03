// MIT License

// This file is a part of longsweep (https://github.com/tmaklin/longsweep)
// Copyright (c) 2020 Tommi Mäklin (tommi@maklin.fi)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <exception>

#include "bxzstr.hpp"

#include "minimap2pseudo.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cerr << "Usage: minimap2pseudo targets.txt input.sam output_prefix" << std::endl;
    return 1;
  }
  bxz::ifstream targets_file(argv[1]);
  if (!targets_file) {
    std::cerr << "Input file " << argv[1] << " is not readable." << std::endl;
    return 1;
  }
  bxz::ifstream sam_file(argv[2]);
  if (!sam_file) {
    std::cerr << "Input file " << argv[2] << " is not readable." << std::endl;
    return 1;
  }
  std::string out_prefix(argv[3]);
  std::ofstream aln_file(out_prefix + ".aln");
  std::ofstream bases_file(out_prefix + "_bases.txt");

  const std::unordered_map<std::string, unsigned> &ref_to_id = ReadTargets(&targets_file);
  std::vector<std::vector<uint32_t>> aln_bases;
  //  const std::vector<std::vector<bool>> &pseudoalignments = ReadSam(ref_to_id, &sam_file);
  const std::vector<std::vector<bool>> &pseudoalignments = ReadSam2(ref_to_id, &sam_file, &aln_bases);
  WritePseudos(pseudoalignments, aln_file);
  WritePseudos(aln_bases, bases_file);
  return 0;
}

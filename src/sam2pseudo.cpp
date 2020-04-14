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
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <exception>

#include "bxzstr.hpp"

std::unordered_map<std::string, unsigned> ReadHeader(std::istream *sam_file) {//, std::vector<std::string> *refs, std::vector<unsigned> *ids) {
  std::unordered_map<std::string, unsigned> ref_to_id;
  std::string line;
  unsigned id = 0;
  bool is_header = true;
  while (getline(*sam_file, line) && is_header) {
    is_header = (line.at(0) == '@' && line.at(1) == 'S' && line.at(2) == 'Q');
    if (is_header) {
      std::stringstream parts(line);
      std::string part;
      getline(parts, part, '\t');
      getline(parts, part, '\t');
      part.erase(part.begin(), part.begin() + 3);
      // refs->emplace_back(part);
      // ids->emplace_back(id);
      ref_to_id.insert(std::make_pair(part, id));
      ++id;
    }
  }
  return ref_to_id;
}

//void ReadSam(const std::unordered_map<std::string, unsigned> &ref_to_id, std::istream *sam_file, std::vector<std::string> *reads, std::vector<std::vector<bool>> *alns) {
std::vector<std::vector<bool>> ReadSam(const std::unordered_map<std::string, unsigned> &ref_to_id, std::istream *sam_file) {
  std::vector<std::vector<bool>> alns;
  std::string line;
  std::string current_read;
  unsigned n_refs = ref_to_id.size();
  while (getline(*sam_file, line)) {
    std::string part;
    std::stringstream parts(line);
    getline(parts, part, '\t');
    if (current_read != part) {
      current_read = part;
      alns.emplace_back(std::vector<bool>(n_refs, false));
    }
    getline(parts, part, '\t');
    getline(parts, part, '\t');
    if (part.at(0) != '*' && part.size() > 1) {
      alns.back()[ref_to_id.at(part)] = true;
    } else {
      alns.pop_back();
    }
  }
  return alns;
}

void WritePseudos(const std::vector<std::vector<bool>> &alns) {
  unsigned n_refs = alns.at(0).size();
  unsigned read_id = 0;
  for (size_t i = 0; i < alns.size(); ++i) {
    std::cout << read_id;
    for (size_t k = 0; k < n_refs; ++k) {
      if (alns[i][k]) {
	std::cout << ' ';
	std::cout << k;
      }
    }
    if (i == alns.size() - 1) {
      std::cout << std::endl;
    } else {
      std::cout << '\n';
    }
    ++read_id;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: sam2pseudo input.sam" << std::endl;
    return 1;
  }
  bxz::ifstream sam_file(argv[1]);
  if (!sam_file) {
    std::cerr << "Input file " << argv[1] << " is not readable." << std::endl;
    return 1;
  }
  const std::unordered_map<std::string, unsigned> &ref_to_id = ReadHeader(&sam_file);//, &ref_names, &ref_ids);
  //  std::vector<std::string> read_names;
  //  ReadSam(ref_to_id, &sam_file, &read_names, &pseudoalignments);
  const std::vector<std::vector<bool>> &pseudoalignments = ReadSam(ref_to_id, &sam_file);
  WritePseudos(pseudoalignments);
  return 0;
}

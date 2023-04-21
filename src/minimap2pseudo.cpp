// MIT License

// This file is a part of sam2pseudo (https://github.com/tmaklin/sam2pseudo)
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

#include "minimap2pseudo.hpp"

#include <iostream>
#include <sstream>
#include <utility>

#include "bxzstr.hpp"

std::unordered_map<std::string, unsigned> ReadTargets(std::istream *targets_file) {
  std::unordered_map<std::string, unsigned> ref_to_id;
  std::string line;
  unsigned id = 0;
  while (getline(*targets_file, line)) {
    ref_to_id.insert(std::make_pair(line, id));
    ++id;
  }
  return ref_to_id;
}

std::vector<std::vector<bool>> ReadSam2(const std::unordered_map<std::string, unsigned> &ref_to_id, std::istream *sam_file, std::vector<std::vector<uint32_t>> *aln_bases) {
  std::vector<std::vector<bool>> alns;
  std::string line;
  std::string current_read;
  unsigned n_refs = ref_to_id.size();
  while (getline(*sam_file, line)) {
    std::string part;
    std::stringstream parts(line);
    getline(parts, part, '\t'); // read name
    if (current_read != part) {
      current_read = part;
      alns.emplace_back(std::vector<bool>(n_refs, false));
      aln_bases->emplace_back(std::vector<uint32_t>(n_refs, 0));
    }
    getline(parts, part, '\t'); // read length
    getline(parts, part, '\t'); // startpos in read
    uint32_t start = std::stoul(part);
    getline(parts, part, '\t'); // endpos in read
    uint32_t end = std::stoul(part);
    getline(parts, part, '\t'); // strand (+ or -)
    getline(parts, part, '\t'); // target sequence name
    if (part.at(0) != '*' && part.size() > 1) {
      unsigned pos_in_ref = ref_to_id.at(part);
      alns.back()[pos_in_ref] = true;
      aln_bases->back()[pos_in_ref] = end - start;
    }
  }
  return alns;
}

std::vector<std::vector<bool>> ReadSam(const std::unordered_map<std::string, unsigned> &ref_to_id, std::istream *sam_file) {
  std::vector<std::vector<bool>> alns;
  std::string line;
  std::string current_read;
  unsigned n_refs = ref_to_id.size();
  while (getline(*sam_file, line)) {
    std::string part;
    std::stringstream parts(line);
    getline(parts, part, '\t'); // read name
    if (current_read != part) {
      current_read = part;
      alns.emplace_back(std::vector<bool>(n_refs, false));
    }
    getline(parts, part, '\t'); // read length
    getline(parts, part, '\t'); // startpos in read
    getline(parts, part, '\t'); // endpos in read
    getline(parts, part, '\t'); // strand (+ or -)
    getline(parts, part, '\t'); // target sequence name
    if (part.at(0) != '*' && part.size() > 1) {
      alns.back()[ref_to_id.at(part)] = true;
    }
    //else {
    //  alns.pop_back();
    //}
  }
  return alns;
}

void WritePseudos(const std::vector<std::vector<bool>> &alns, std::ostream &out) {
  unsigned n_refs = alns.at(0).size();
  unsigned read_id = 0;
  for (size_t i = 0; i < alns.size(); ++i) {
    out << read_id;
    for (size_t k = 0; k < n_refs; ++k) {
      if (alns[i][k]) {
	out << ' ';
	out << k;
      }
    }
    if (i == alns.size() - 1) {
      out << std::endl;
    } else {
      out << '\n';
    }
    ++read_id;
  }
}

void WritePseudos(const std::vector<std::vector<uint32_t>> &alns, std::ostream &out) {
  unsigned n_refs = alns.at(0).size();
  unsigned read_id = 0;
  for (size_t i = 0; i < alns.size(); ++i) {
    out << read_id;
    for (size_t k = 0; k < n_refs; ++k) {
      out << ' ';
      out << alns[i][k];
    }
    if (i == alns.size() - 1) {
      out << std::endl;
    } else {
      out << '\n';
    }
    ++read_id;
  }
}

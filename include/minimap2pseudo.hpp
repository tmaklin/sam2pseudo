#ifndef LONGSWEEP_MINIMAP2PSEUDO_HPP
#define LONGSWEEP_MINIMAP2PSEUDO_HPP

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>

std::unordered_map<std::string, unsigned> ReadTargets(std::istream *targets_file);
std::vector<std::vector<bool>> ReadSam2(const std::unordered_map<std::string, unsigned> &ref_to_id, std::istream *sam_file, std::vector<std::vector<uint32_t>> *aln_bases);
void WritePseudos(const std::vector<std::vector<bool>> &alns, std::ostream &out);
void WritePseudos(const std::vector<std::vector<uint32_t>> &alns, std::ostream &out);

#endif

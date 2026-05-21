#ifndef STANDARD_ALGORITHMS_H
#define STANDARD_ALGORITHMS_H

#include <vector>
#include <string>

bool lessThan(const std::string& a, const std::string& b, long long& cnt);
bool lessThanOrEqual(const std::string& a, const std::string& b, long long& cnt);
void standardQuickSort(std::vector<std::string>& a, int l, int r, long long& cnt);
void standardMergeSort(std::vector<std::string>& a, int l, int r, long long& cnt);

#endif
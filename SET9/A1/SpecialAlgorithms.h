#ifndef SPECIAL_ALGORITHMS_H
#define SPECIAL_ALGORITHMS_H

#include <vector>
#include <string>

void stringQuickSort(std::vector<std::string>& arr, int low, int high, int d, long long& comps);
void stringMergeSort(std::vector<std::string>& arr, long long& comps);
void msdRadixSort(std::vector<std::string>& arr, long long& comps);
void msdRadixSortWithCutoff(std::vector<std::string>& arr, long long& comps);

#endif
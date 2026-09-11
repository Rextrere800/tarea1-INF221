#pragma once
#include <vector>
#include "mergesort.hpp"

std::vector<int> mergeSortReal(std::vector<int> arr) {
    if (arr.empty()) return arr;
    mergeSort(arr, 0, arr.size() - 1);
    return arr;
}
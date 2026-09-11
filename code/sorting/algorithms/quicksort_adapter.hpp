#pragma once
#include <vector>
#include "quicksort.hpp"

std::vector<int> quickSortReal(std::vector<int> arr) {
    if (arr.empty()) return arr;
    quickSort(arr, 0, arr.size() - 1);
    return arr;
}
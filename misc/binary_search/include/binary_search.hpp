#pragma once

#include<vector>
#include<utility>

using std::vector;
using std::swap;

template<typename T>
int binary_search(const vector<T>& sorted_data, T target) {
    int left = 0, right = sorted_data.size() - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (sorted_data[middle] < target) {
            left = middle + 1;
        }
        else if (sorted_data[middle] > target) {
            right = middle - 1;
        }
        else {
            return middle;
        }
    }

    return -1;
}
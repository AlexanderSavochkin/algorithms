#pragma once

#include <vector>
#include <algorithm>
#include <stack>
#include <tuple>

using std::vector;
using std::swap;
using std::stack;
using std::tuple;
using std::make_tuple;
using std::tie;

#include <iostream>
using std::cout;
using std::endl;


template<typename T>
size_t partition(vector<T>& arr)
{
    size_t hi = arr.size() - 1, lo = 1;
    while (lo < hi) {
        if (arr[lo] <= arr[0]) {
            ++lo;
            continue;
        }

        if (arr[hi] > arr[0]) {
            --hi;
            continue;
        }

        swap(arr[hi], arr[lo]);
    }
    return arr[lo] > arr[0] ? lo - 1 : lo;
}

template<typename T>
size_t partition_range(vector<T>& arr, size_t from, size_t to)
{
    size_t hi = to, lo = from + 1;
    while (lo < hi) {
        if (arr[lo] <= arr[from]) {
            ++lo;
            continue;
        }

        if (arr[hi] > arr[from]) {
            --hi;
            continue;
        }

        swap(arr[hi], arr[lo]);
    }
    size_t split_point = arr[lo] > arr[from] ? lo - 1 : lo;
    swap(arr[split_point], arr[from]);
    return split_point;
}

template<typename T>
void my_quick_sort(vector<T>& arr)
{
    stack<tuple<size_t, size_t>> st;
    st.push(make_tuple(0, arr.size() - 1));

    while (!st.empty()) 
    {
        size_t from, to;
        tie(from, to) = st.top();
        st.pop();

        size_t split_point = partition_range(arr, from, to);

        if (split_point > 0 && from < split_point - 1)
        {
            st.push(make_tuple(from, split_point - 1));
        }

        if (to > split_point + 1)
        {
            st.push(make_tuple(split_point + 1, to));
        }
    }
}

template<typename T>
void my_quick_select(vector<T>& arr, int k)
{
    int lo = 0, hi = arr.size() - 1;
    while (hi - lo > 1) 
    {
        size_t split_point = partition_range(arr, lo, hi);

        if (split_point == k)
            return;

        if (split_point < k)
            lo = split_point + 1;

        if (split_point > k)
            hi = split_point - 1;
    }
}


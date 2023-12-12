#pragma once

#include <vector>

using std::vector;

template<typename T>
class BitIndexedTree
{
    vector<T> tree;

    int parent_node_index(int i)
    {
        //For parent node we remove least significant bit of the current index, so parent of 0101b is 0100b
        return i - (i & (-i));
    }

    int next_node_index(int i)
    {
        return i + (i & (-i));
    }

public:

    BitIndexedTree(int sz)
        :tree(0, sz) {}

    BitIndexedTree(const vector<T>& data)
        :tree(data.size(), 0)
    {
        for (int i = 0; i < data.size(); ++i)
            update_delta(i, data[i]);
    }

    T getSumToIndex(int i)
    {
        T sum = T();
        ++i;
        while (i > 0) {
            sum += tree[i];
            i = parent_node_index(i);
        }

        return sum;
    }

    void update_delta(int i, T dv)
    {
        ++i;
        while (i < tree.size()) {
            tree[i] += dv;
            i = next_node_index(i);
        }
    }

    void update(int i, T v)
    {
        T old = getSumToIndex(i) - getSumToIndex(i - 1);
        T dv = v - old;
        update_delta(i, dv);
    }
};
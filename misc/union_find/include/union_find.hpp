#pragma once

class UnionFindByRank
{
    vector<int> root;
    vector<int> rank;
public:
    UnionFindByRank(int size) : root(size), rank(size, 0) {
        for (int i = 0; i < size; ++i) {
            root[i] = i;
        }
    }

    int find(int i) {
        while (root[i] != i) {
            i = root[i];
        }

        return i;
    }

    void union_elements(int i, int j) {
        int repr_i = find(i);
        int repr_j = find(j);
        if (repr_i == repr_j) {
            return;
        }

        if (rank[repr_i] < rank[repr_j]) {
            root[repr_i] = repr_j;
        }
        else if (rank[repr_i] > rank[repr_j]) {
            root[repr_j] = repr_i;
        } else {
          root[repr_j] = repr_i;
          ++rank[repr_i];
        }
    }
};

#pragma once

#include <vector>
#include <cmath>

using std::vector;
using std::max;
using std::min;

class SegmentTree
{
    vector<int> nums;
    vector<int> segment_tree;

    int buildSegmentTree(int tree_node_index, int left, int right)
    {
        if (right == left)
        {
            segment_tree[tree_node_index] = nums[left];
            return segment_tree[tree_node_index];
        }
        else
        {
            int middle = (left + right) / 2;
            int left_sum = buildSegmentTree(2 * tree_node_index + 1, left, middle);
            int right_sum = buildSegmentTree(2 * tree_node_index + 2, middle + 1, right);
            segment_tree[tree_node_index] = left_sum + right_sum;
            return left_sum + right_sum;
        }
    }

    int sumRange(int i, int j, int tree_left, int tree_right, int tree_node_index)
    {
        int middle = (tree_left + tree_right) / 2;
        int left_index = 2 * tree_node_index + 1;
        int right_index = 2 * tree_node_index + 2;
        int result = 0;

        if (i == tree_left && j == tree_right)
        {
            return segment_tree[tree_node_index];
        }

        if (middle >= i)
        {
            result += sumRange(i, min(j, middle), tree_left, middle, 2 * tree_node_index + 1);
        }

        if (middle + 1 <= j)
        {
            result += sumRange(max(i, middle + 1), j, middle + 1, tree_right, 2 * tree_node_index + 2);
        }

        return result;
    }

    int updateRange(int i, int val, int left, int right, int tree_node_index)
    {
        int middle = (left + right) / 2;
        int left_index = 2 * tree_node_index + 1;
        int right_index = 2 * tree_node_index + 2;
        int result = 0;

        if (right == left)
        {
            int old_val = segment_tree[tree_node_index];
            segment_tree[tree_node_index] = val;
            nums[i] = val;
            return val - old_val;
        }

        int change;
        if (middle >= i)
        {
            change = updateRange(i, val, left, middle, 2 * tree_node_index + 1);
        }
        else
        {
            change = updateRange(i, val, middle + 1, right, 2 * tree_node_index + 2);
        }
        segment_tree[tree_node_index] += change;
        return change;
    }

public:
    SegmentTree(vector<int> &nums) : nums(nums)
    {
        if (nums.size() == 0)
        {
            return;
        }

        // Height of segment tree
        int x = (int)(ceil(log2(nums.size())));

        // Maximum size of segment tree
        int max_size = 2 * (int)pow(2, x) - 1;

        segment_tree.resize(max_size);

        buildSegmentTree(0, 0, nums.size() - 1);
    }

    void update(int i, int val)
    {
        if (nums.size() == 0)
        {
            return;
        }

        updateRange(i, val, 0, nums.size() - 1, 0);
    }

    int sumRange(int i, int j)
    {
        if (nums.size() == 0)
        {
            return 0;
        }
        
        return sumRange(i, j, 0, nums.size() - 1, 0);
    }
};
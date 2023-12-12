#pragma once

#include <functional>
#include <stack>

#include "../../../common/tree.hpp"

using std::function;
using std::stack;

void inorder_traverse(TreeNode *root, function<void(TreeNode*)> f)
{
    if (root == nullptr)
    {
        return;
    }

    stack<TreeNode *> st;
    TreeNode* current = root;

    while (!st.empty() || current != nullptr)
    {
        while (current != nullptr)
        {
            st.push(current);
            current = current->left;
        }

        current = st.top();
        st.pop();
        f(current);
        current = current->right;
    }
}

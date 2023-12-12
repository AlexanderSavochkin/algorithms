#pragma once

#include <functional>
#include <stack>

#include "../../../common/tree.hpp"

using std::function;
using std::stack;

void preorder_traverse(TreeNode *root, function<void(TreeNode*)> f)
{
    if (root == nullptr)
    {
        return;
    }

    stack<TreeNode *> st;
    st.push(root);

    while(!st.empty())
    {
        auto current = st.top();
        st.pop();

        f(current);

        if (current->right != nullptr)
        {
            st.push(current->right);
        }

        if (current->left != nullptr)
        {
            st.push(current->left);
        }
    }
}

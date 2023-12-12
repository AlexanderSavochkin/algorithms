#include "../include/inorder_traverse.hpp"
#include "../../../../test/catch_amalgamated.hpp"

#include <vector>

using std::vector;


bool check_inorder_traversal() 
{
    TreeNode *root = new TreeNode(5);
    root->left = new TreeNode(2);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);
    root->right = new TreeNode(8);
    root->right->left = new TreeNode(7);
    root->right->right = new TreeNode(11);

    vector<int> expected_order = {1, 2, 3, 5, 7, 8, 11};
    vector<int> actual_order;

    inorder_traverse(root, [&actual_order](TreeNode* node) { actual_order.push_back(node->val); });

    if (actual_order.size() != expected_order.size())
    {
        return false;
    }

    for (int i = 0; i < actual_order.size(); ++i)
    {
        if (actual_order[i] != expected_order[i])
        {
            return false;
        }
    }   

    return true;
}


TEST_CASE("Test inorder traversal", "[Inorder]") {
    REQUIRE(check_inorder_traversal());
}



#include "../include/preorder_traverse.hpp"
#include "../../../../test/catch_amalgamated.hpp"

#include <vector>

using std::vector;


bool check_preorder_traversal() 
{
    TreeNode *root = new TreeNode(5);
    root->left = new TreeNode(2);
    root->right = new TreeNode(8);
    root->right->left = new TreeNode(7);
    root->right->right = new TreeNode(11);

    vector<int> expected_order = {5, 2, 8, 7, 11};
    vector<int> actual_order;

    preorder_traverse(root, [&actual_order](TreeNode* node) { actual_order.push_back(node->val); });

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


TEST_CASE("Test preorder traversal", "[Preorder]") {
    REQUIRE(check_preorder_traversal());
}

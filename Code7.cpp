/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* mergeTrees(const TreeNode* root1, const TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            root1 = &dummy_;
        }
        if (!root2) {
            root2 = &dummy_;
        }

        std::deque<tuple<const TreeNode*, const TreeNode*, TreeNode**>> current_nodes_and_new_node;
        TreeNode* new_root = new TreeNode();
        current_nodes_and_new_node.push_back({root1, root2, &new_root});
        while (!current_nodes_and_new_node.empty()) {
            auto [node1, node2, ptr_to_new_node] = current_nodes_and_new_node.front();
            current_nodes_and_new_node.pop_front();
            if (!node1 && !node2) {
                *ptr_to_new_node = nullptr;
                continue;
            }
            if (!node1) {
                node1 = &dummy_;
            }
            if (!node2) {
                node2 = &dummy_;
            }
            (*ptr_to_new_node)->val = node1->val + node2->val;
            (*ptr_to_new_node)->left = new TreeNode();
            current_nodes_and_new_node.push_back({node1->left, node2->left, &((*ptr_to_new_node)->left)});
            (*ptr_to_new_node)->right = new TreeNode();
            current_nodes_and_new_node.push_back({node1->right, node2->right, &((*ptr_to_new_node)->right)});
        }
        return new_root;
    }
private:
    inline static const TreeNode dummy_{0};
};

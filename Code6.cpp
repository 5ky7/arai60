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

        std::deque<tuple<const TreeNode*, const TreeNode*, TreeNode*>> current_nodes_and_new_node;
        TreeNode* new_root = new TreeNode();
        current_nodes_and_new_node.push_back({root1, root2, new_root});
        while (!current_nodes_and_new_node.empty()) {
            auto [node1, node2, new_node] = current_nodes_and_new_node.front();
            current_nodes_and_new_node.pop_front();
            if (!node1) {
                node1 = &dummy_;
            }
            if (!node2) {
                node2 = &dummy_;
            }
            new_node->val = node1->val + node2->val;
            if (node1->left || node2->left) {
                new_node->left = new TreeNode();
                current_nodes_and_new_node.push_back({node1->left, node2->left, new_node->left});
            }
            if (node1->right || node2->right) {
                new_node->right = new TreeNode();
                current_nodes_and_new_node.push_back({node1->right, node2->right, new_node->right});
            }
        }
        return new_root;
    }
private:
    static const TreeNode dummy_;
};

const TreeNode Solution::dummy_{0};

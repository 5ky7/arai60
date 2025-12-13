class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return MergeTreesHelper(root1, root2);
    }
private:
    const TreeNode dummy_ = TreeNode();

    TreeNode* MergeTreesHelper(const TreeNode* root1, const TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            root1 = &dummy_;
        }
        if (!root2) {
            root2 = &dummy_;
        }

        TreeNode* new_root = new TreeNode(root1->val + root2->val);
        new_root->left = MergeTreesHelper(root1->left, root2->left);
        new_root->right = MergeTreesHelper(root1->right, root2->right);
        return new_root;
    }
};

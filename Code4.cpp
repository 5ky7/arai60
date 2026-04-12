class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return MergeTreesHelper(root1, root2);
    }
private:
    TreeNode* MergeTreesHelper(const TreeNode* root1, const TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }

        // root1もroot2も存在するなら新しいrootを作成して返す
        TreeNode* new_root = new TreeNode(root1->val + root2->val);
        new_root->left = MergeTreesHelper(root1->left, root2->left);
        new_root->right = MergeTreesHelper(root1->right, root2->right);
        return new_root;
    }
};

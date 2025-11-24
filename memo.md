# Step 1
* 考えたがわからなかったので過去の提出を確認
  * あるnodeに対して，node->leftにはnode->valより小さい値のみ，node->rightにはnode->valより大きい値のみが入っている
  * これって2分探索そのままだ．numsが昇順であることとピッタリ整合
  * Helper関数にnodeとnumsを渡しておけば良い
### Code1
```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return Helper(nums, 0, nums.size() - 1);
    }
private:
    TreeNode* Helper(const vector<int>& nums, int left, int right) {
        if (left > right) {
            return nullptr;
        }

        int center = (left + right) / 2;
        TreeNode* root = new TreeNode(nums[center]);
        root->left = Helper(nums, left, center - 1);
        root->right = Helper(nums, center + 1, right);

        return root;
    }
};
```
# Step 2

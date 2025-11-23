# Step 1
* 「今のnodeとleft, rightを比べて大小満たしていれば一旦OK．あとはleft, rightがvalid化見れば良い．」と考えたのが[Code1](#Code1)
  * しかしこれだとダメで，というのもnode->right以下のsubtreeにnodeより小さいnodeが含まれている場合を逃してしまう．
* 

```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        bool left_flag;
        bool right_flag;

        if (root->left) {
            if (root->val <= root->left->val) {
                return false;
            }
            left_flag = isValidBST(root->left);
        } else {
            left_flag = true;
        }

        if (root->right) {
            if (root->val >= root->right->val) {
                return false;
            }
            right_flag = isValidBST(root->right);
        } else {
            right_flag = true;
        }

        return left_flag && right_flag;
    }
};
```
# Step 2

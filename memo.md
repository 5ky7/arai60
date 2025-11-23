# Step 1
* 「今のnodeとleft, rightを比べて大小満たしていれば一旦OK．あとはleft, rightがvalid化見れば良い．」と考えたのが[Code1](#Code1)
  * しかしこれだとダメで，というのもnode->right以下のsubtreeにnodeより小さいnodeが含まれている場合を逃してしまう．
* node->left以下のsubtreeにとってはnode->valが値の上界，node->right以下のsubtreeにとってはnode->valが値の下界なので，上界と下界を持った再帰でかけば良いと考えて[Code2](#Code2)

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
### Code2
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return Helper(root, nullptr, nullptr);
    }
private:
    bool Helper(TreeNode* root, TreeNode* high, TreeNode* low) {
        if (!root) {
            return true;
        }

        if (low && low->val >= root->val) return false;
        if (high && high->val <= root->val) return false;

        return Helper(root->left, root, low) && Helper(root->right, high, root);
    }
};
```
# Step 2

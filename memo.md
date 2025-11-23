# Step 1
* 入力のnodeから葉までの長さを返す再帰関数でleftとrightを見ていけばいいと考えた．
  * なんとなく現時点での深さ`current_depth`を引数に与えて関数を書き上げた([Code1](#Code1))が，必要ないことに気づいた([Code2](#Code2))
### Code1
```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        return GetDepth(root, 1);
    }
private:
    int GetDepth(TreeNode* node, int current_depth) {
        if (node == nullptr) {
            return 0;
        }
        ++current_depth;
        return 1 + max(GetDepth(node->left, current_depth), GetDepth(node->right, current_depth));
    }
};
```
### Code2
```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        return GetDepth(root);
    }
private:
    int GetDepth(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + max(GetDepth(node->left), GetDepth(node->right));
    }
};
```

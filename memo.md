# Step 1
* 「今のnodeとleft, rightを比べて大小満たしていれば一旦OK．あとはleft, rightがvalid化見れば良い．」と考えたのが[Code1](#Code1)
  * しかしこれだとダメで，というのもnode->right以下のsubtreeにnodeより小さいnodeが含まれている場合を逃してしまう．
* node->left以下のsubtreeにとってはnode->valが値の上界，node->right以下のsubtreeにとってはnode->valが値の下界なので，上界と下界を持った再帰でかけば良いと考えて[Code2](#Code2)

### Code1
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
        return isValidBSTHelper(root, nullptr, nullptr);
    }

private:
    bool isValidBSTHelper(TreeNode* root, TreeNode* upper_bound, TreeNode* lower_bound) {
        if (!root) {
            return true;
        }
        if (upper_bound && upper_bound->val <= root->val) {
            return false;
        }
        if (lower_bound && lower_bound->val >= root->val) {
            return false;
        }
        return isValidBSTHelper(root->left, root, lower_bound) && isValidBSTHelper(root->right, upper_bound, root);
    }
};
```
# Step 2
* `isValidBSTHelper`の引数に値の上界と下界を与えるためにTreeNode*を渡しているが，これを無理やり数値にすると[Code3](#Code3)になる．
  * ただし`node->val`の値の範囲を元にマジックナンバー的に上界と下界の初期値を与えているので
* [通りがけ順の巡回](https://discord.com/channels/1084280443945353267/1192736784354918470/1234120299008491581)を利用する．[Code4](#Code4)．
  * 通りがけ順にvectorにnode->valを格納していって，最後に`std::is_sorted()`を利用したが，これだと`strictly less than`でなく等号成立も許可するのでダメだった．自前で判定関数を用意した．
* [BFSの利用](https://github.com/olsen-blue/Arai60/pull/28)もあったのでやってみる．[Code5](#Code5)．
  * なんか変に難しく考えてたが，「各nodeについてBST条件を満たすか」をチェックするだけだから全てのnodeをtraverseできればBFSでもDFSでも実装可能．


### Code3
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        long long min = static_cast<long long>(numeric_limits<int>::min()) - 1;
        long long max = static_cast<long long>(numeric_limits<int>::max()) + 1;
        return IsValidBSTHelper(root, min, max);
    }

private:
    bool IsValidBSTHelper(TreeNode* root, long long lower_bound, long long upper_bound) {
        if (!root) {
            return true;
        }

        if (root->val <= lower_bound) {
            return false;
        }
        if (root->val >= upper_bound) {
            return false;
        }

        return IsValidBSTHelper(root->left, lower_bound, root->val) && IsValidBSTHelper(root->right, root->val, upper_bound);
    }
};
```

### Code4
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        vector<int> values_inorder;
        TraverseInorder(root, values_inorder);
        return CheckBSTByVector(values_inorder);
    }

private:
    void TraverseInorder(TreeNode* root, vector<int>& values_inorder) {
        if (!root) {
            return;
        }

        if (root->left) {
            TraverseInorder(root->left, values_inorder);
        }

        values_inorder.push_back(root->val);

        if (root->right) {
            TraverseInorder(root->right, values_inorder);
        }
    }

    bool CheckBSTByVector(const vector<int>& vec) {
        auto ptr = vec.begin();
        while (ptr + 1 < vec.end()) {
            if (*ptr >= *(ptr + 1)) {
                return false;
            }
            ++ptr;
        }
        return true;
    }
};
```

### Code5
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (!root) {
            return true;
        }

        std::deque<NodeAndRange> frontier;
        frontier.push_back({root, nullptr, nullptr});
        while (!frontier.empty()) {
            std::deque<NodeAndRange> next_frontier;
            while (!frontier.empty()) {
                auto [node, upper_bound, lower_bound] = frontier.front();
                frontier.pop_front();
                if (!node) {
                    continue;
                }
                if (upper_bound && node->val >= upper_bound->val) {
                    return false;
                }
                if (lower_bound && node->val <= lower_bound->val) {
                    return false;
                }
                next_frontier.push_back({node->left, node, lower_bound});
                next_frontier.push_back({node->right, upper_bound, node});
            }
            frontier = std::move(next_frontier);
        }

        return true;
    }

private:
    struct NodeAndRange {
        TreeNode* node;
        TreeNode* upper_bound;
        TreeNode* lower_bound;
    };
};
```

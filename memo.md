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

# Step 2
* Step1から時間が経ったのでとりあえず解き直してみる([Code3](#Code3)．所要時間4分くらい．
* 再帰をループで書いているものがあったので自分もやってみる（[Code4](#Code4)）．所要時間9分．
  * popし忘れて一敗．
  * `traversing_node`を`deque<TreeNode*>`として宣言して書き進めて，「あれ，深さ取得できないじゃん」となって`struct node_and_depth`を定義して完成
    * 多分「あれ，深さ取得できないじゃん」となる前に最初から「深さ持っておかなきゃ」と思えるようにはある程度経験が必要そう
  * BFSでやっているものもある
    * [Code4](#Code4)の`push_front()`を`push_back()`に置き換えれば良い
    * BFSの場合は`max_depth = std::max(max_depth, depth);`は常に更新される
    * `while(!~.empty())`を`while(true)`にして，このループの末尾に`if (~.empty) {max_depth = depth; break;}`を入れても良いかも
  * [Nullチェックはpop直後にもできる](https://github.com/Apo-Matchbox/LeetCode_Practice/pull/29/files#r2596191790)という発想は自分にはなかった．
    * 「チェック→追加→取得→処理」，「追加→取得→チェック→処理」の違い．結局処理の前にチェックが入れば良い．
  * （BFSの時に）深さを管理したい場合は，[異なる深さのノードを異なる場所に格納する](https://github.com/Apo-Matchbox/LeetCode_Practice/pull/29/files#r2596188103)とわかりやすい．([Code5](#Code5))

### Code3
```cpp
class Solution {
private:
    int DepthFromNode(TreeNode* node) {
        if (!node) {
            return 0;
        }
        return 1 + std::max(DepthFromNode(node->left), DepthFromNode(node->right));
    }
public:
    int maxDepth(TreeNode* root) {
        return DepthFromNode(root);
    }
};
```

### Code4
```cpp
class Solution {
private:
    struct node_and_depth{
        TreeNode* node;
        int depth;
    };
public:
    int maxDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }
        std::deque<node_and_depth> traversing_node;
        traversing_node.push_front({root, 1});
        int max_depth = 0;
        while (!traversing_node.empty()) {
            auto [node, depth] = traversing_node.front();
            traversing_node.pop_front();
            max_depth = std::max(max_depth, depth);
            if (node->right) {
                traversing_node.push_front({node->right, depth + 1});
            }
            if (node->left) {
                traversing_node.push_front({node->left, depth + 1});
            }
        }

        return max_depth;
    }
};
```

### Code5
```cpp
class Solution {
private:
    struct node_and_depth{
        TreeNode* node;
        int depth;
    };
public:
    int maxDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }
        std::deque<TreeNode*> traversing_nodes;
        std::deque<TreeNode*> next_level_nodes;
        traversing_nodes.push_back(root);
        int depth = 0;
        while (!traversing_nodes.empty()) {
            ++depth;
            while(!traversing_nodes.empty()) {
                TreeNode* node = traversing_nodes.front();
                traversing_nodes.pop_front();
                if (node->right) {
                    next_level_nodes.push_back(node->right);
                }
                if (node->left) {
                    next_level_nodes.push_back(node->left);
                }
            }
            traversing_nodes = std::move(next_level_nodes);
        }

        return depth;
    }
};
```

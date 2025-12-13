# Step 1
* Maximum Depthのmaxをminにしただけ，と思って書いたがWA（[Code1](#Code1)）
  * これだと，入力`node`に対して，leftがnullptrでrightに実ノードを持つとき，leftから0が帰って来て`GetDepth(node)`の返り値が1になる
  * しかし本来返り値になるべきは"`node`から葉までの距離"であるから，leftもrightもnullptrの時のみ`GetDepth(node)`の返り値が1になって欲しい
* ということで修正したのが[Code2](#Code2)
  * しかしこれもWAで，よく見たら`root = nullptr`の可能性もあり，この時にエラーを吐いていた
* ということで[Code3](#Code3)のようにしたらAccept
  * 問題設定に不満を覚えかけたが，コーナーケースを考えるのは自分の仕事であり，入力の確認を怠った自分が悪いと反省．
  * そもそも[Code1](#Code1)のミスは，頭でテストケースを使ってデバッグできていない証拠
* しかし[Code3](#Code3)もあまり美しくない...もっとスッキリかけそう
### Code1
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        return GetDepth(root);
    }
private:
    int GetDepth(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }

        return 1 + min(GetDepth(node->left), GetDepth(node->right));
    }
};
```
### Code2
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        return GetDepthToLeaf(root);
    }
private:
    int GetDepthToLeaf(TreeNode* node) {
        if (!node->left && !node->right) {
            return 1;
        }
        
        int length_to_leaf = INT_MAX;
        if (node->left) {
            length_to_leaf = 1 + GetDepthToLeaf(node->left);
        }
        if (node->right) {
            length_to_leaf = min(length_to_leaf, 1 + GetDepthToLeaf(node->right));
        }

        return length_to_leaf;
    }
};
```
### Code3
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        
        return GetDepthToLeaf(root);
    }
private:
    int GetDepthToLeaf(TreeNode* node) {
        if (!node->left && !node->right) {
            return 1;
        }
        
        int length_to_leaf = INT_MAX;
        if (node->left) {
            length_to_leaf = 1 + GetDepthToLeaf(node->left);
        }
        if (node->right) {
            length_to_leaf = min(length_to_leaf, 1 + GetDepthToLeaf(node->right));
        }

        return length_to_leaf;
    }
};
```
# Step 2
* 最初に解いてから時間が経ったので解き直してみる([Code4](#Code4)）
  * 所要時間6分．
* 再帰をループで書いてみる
  * ループで書こうとしたところで，BFSにすれば最初に葉を見つけた瞬間に終われば良いと気づく（[Code5](#Code5)）
* [下から集める（関数の返り値で上に伝える）か，上から渡す（関数の引数で下に伝える）か](https://discord.com/channels/1084280443945353267/1196472827457589338/1237988315781664770)という観点があったので，上から渡す方法でもやってみる([Code6](#Code6))．
  * `node->left`, `node->right`について同じコードを書いていたので，[一つにまとめてみた](https://github.com/Yoshiki-Iwasa/Arai60/pull/25#discussion_r1665958206)．

### Code4
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }
        return GetMinDepth(root);
    }
private:
    int GetMinDepth(TreeNode* root) {
        if (!root->left && !root->right) {
            return 1;
        }

        int min_child_depth = INT_MAX;
        if (root->left) {
            min_child_depth = std::min(min_child_depth, GetMinDepth(root->left));
        }
        if (root->right) {
            min_child_depth = std::min(min_child_depth, GetMinDepth(root->right));
        }
        
        return min_child_depth + 1;
    }
};
```

### Code5
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }
        std::deque<TreeNode*> found_nodes;
        found_nodes.push_back(root);
        int depth = 0;
        while (!found_nodes.empty()) {
            ++depth;
            bool leaf_exist = false;
            std::deque<TreeNode*> next_level_nodes;
            while (!found_nodes.empty()) {
                TreeNode* node = found_nodes.front();
                found_nodes.pop_front();
                if (!node->left && !node->right) {
                    // 葉を見つけたら終了
                    leaf_exist = true;
                    break;
                }
                if (node->left) {
                    next_level_nodes.push_back(node->left);
                }
                if (node->right) {
                    next_level_nodes.push_back(node->right);
                }
            }
            if (leaf_exist) {
                // 葉を見つけたら終了
                break;
            }
            found_nodes = std::move(next_level_nodes);
        }
        return depth;
    }
};
```

### Code6
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }
        int min_depth = INT_MAX;
        MinDepthHelper(root, 1, min_depth);
        return min_depth;
    }
private:
    void MinDepthHelper(const TreeNode* node, const int depth, int& min_depth) {
        if (!node->left && !node->right) {
            min_depth = std::min(depth, min_depth);
            return;
        }

        for (const TreeNode* child : {node->left, node->right}) {
            if (child) {
                MinDepthHelper(child, depth + 1, min_depth);
            }
        }
    }
};
```

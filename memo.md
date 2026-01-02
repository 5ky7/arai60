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
  * ただし`node->val`の値の範囲を元にマジックナンバー的に上界と下界の初期値を与えていることに注意．
* [通りがけ順の巡回](https://discord.com/channels/1084280443945353267/1192736784354918470/1234120299008491581)を利用する．[Code4](#Code4)．
  * 通りがけ順にvectorにnode->valを格納していって，最後に`std::is_sorted()`を利用したが，これだと`strictly less than`でなく等号成立も許可するのでダメだった．自前で判定関数を用意した．
* [BFSの利用](https://github.com/olsen-blue/Arai60/pull/28)もあったのでやってみる．[Code5](#Code5)．
  * なんか変に難しく考えてたが，「各nodeについてBST条件を満たすか」をチェックするだけだから全てのnodeをtraverseできればBFSでもDFSでも実装可能．
* [Generatorの利用](https://github.com/fhiyo/leetcode/pull/30)．[Code6](#Code6)
  * ジェネレータとは？
    * 通常の関数：呼び出される --> 最後まで実行 --> 値を返して終了
    * ジェネレータ：呼び出される --> 値を返して一時停止 --> 再度呼ばれるとそこから再開
    * `co_yield`で値を返して一時停止，`co-return`で完全終了
  * 今回で言うと，[通りがけ順の巡回](#Code4)と組み合わせることで，~空間計算量を定数化できる~（配列に`node->val`を格納せず，逐一`node->val`を比較できる）．
    * 状態を一時的に保存する，かつ，入れ子構造になっているので，最悪O(n)の空間計算量が必要．
  * `std::ranges::elements_of()`の役割：
    * ジェネレータ`InorderValues`は入れ子構造になっている
    * 入れ子として中にある`InorderValues`が`co_yield`するたびに，その値を（一つずつ）取得する．
    * それを親ジェネレータの`co_yield`の対象とする．
* [Morris Traversal](https://discord.com/channels/1084280443945353267/1200089668901937312/1213356258103525407)の利用．
  * 空間計算量を定数に保ったまま，Inorder探索ができる --> BSTの判定もできる．
  * Morris Traversalのポイント：
    * 今見ている`current_node`は先頭か？
      * `current_node->left`がないなら`current_node`が先頭．後は`current_node->right`以下を見ていけば良い．
      * `current_node->left`があるなら，`current_node->left`を`current_node`として，同じことをする．
        * ただし，`current_node`の直前に来るやつにはnodeへのリンクを貼っておく（temporary link）．
          * なぜなら，morris traversalにおいては基本的に自分より下にあるノードにしか進んでいかないため，`current_node`へ戻る道を設定しておく必要がある．
          * inorderで，（leftを持っている）`current_node`の直前に来るノード(`nodeA`とする)はrightを持たない．
            * rightを持っていたら，`current_node`は少なくとも`nodeA->right`よりinorderで後ろにくる．つまり`current_node`の直前が`nodeA`にならない．
        * 先頭が確定したらrightに進むので，いつかtemporary linkを踏んで元のnodeに戻る．この時に元の`current_node`の順番が確定する．
  * ということで書いたがエラーが出た．バグはなさそうなんだがなあと思っていたが，どうやら入力を破壊的に変更してそれを戻す前にearly returnしたために，leet code実行環境の木構造のデストラクタでエラーが起きているようだ．
  * というわけで，early returnはできない．最後までtraverseしてからtrue, falseを返すように修正した．[Code7](#Code7)


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

### Code6
```cpp
#include <generator>

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        long long previous_val = std::numeric_limits<long long>::min();
        for (long long current_val : InorderValues(root)) {
            if (current_val <= previous_val) {
                return false;
            }
            previous_val = current_val;
        }
        return true;
    }

private:
    std::generator<long long> InorderValues(TreeNode* node) {
        if (!node) {
            co_return;
        }

        co_yield std::ranges::elements_of(InorderValues(node->left));
        co_yield static_cast<long long>(node->val);
        co_yield std::ranges::elements_of(InorderValues(node->right));
    }
};
```

### Code7
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (!root) {
            return true;
        }

        TreeNode* current_node = root;
        TreeNode* previous_node_inorder = nullptr;
        bool is_valid_BST = true;
        while (current_node) {
            auto [predecessor_in_left_subtree, temporary_link] = FindPredecessorInLeftSubtree(current_node);
            if (!predecessor_in_left_subtree) {
                // left subtreeが存在しない時，current_nodeは現時点で先頭（確定）．
                // 次に進む = rightを見ていく．
                if (previous_node_inorder && previous_node_inorder->val >= current_node->val) {
                    is_valid_BST = false;
                }
                previous_node_inorder = current_node;
                current_node = current_node->right;
                continue;
            }
            if (!temporary_link) {
                // left subtreeが存在するが，temporary linkが作成されていない時，
                // current_nodeは現時点で先頭ではない．
                // temporary_linkを作成して，leftを見ていく．
                predecessor_in_left_subtree->right = current_node;
                current_node = current_node->left;
                continue;
            }
            // left subtreeが存在し，predecessorからcurrent_nodeへのtemporary linkが存在する時，
            // current_nodeのleft subtreeは探索終了．つまりcurrent_nodeは現時点で先頭（確定）．
            predecessor_in_left_subtree->right = nullptr;
            if (previous_node_inorder && previous_node_inorder->val >= current_node->val) {
                is_valid_BST = false;
            }
            previous_node_inorder = current_node;
            current_node = current_node->right;
        }

        return is_valid_BST;
    }

private:
    std::pair<TreeNode*, bool> FindPredecessorInLeftSubtree(TreeNode* node) {
        if (!node->left) {
            return {nullptr, false};
        }

        TreeNode* predecessor = node->left;
        while (predecessor->right) {
            if (predecessor->right == node) {
                return {predecessor, true};
            }
            predecessor = predecessor->right;
        }
        return {predecessor, false};
    }
};
```

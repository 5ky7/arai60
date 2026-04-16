# Step 1
* 普通に解いてみる．所要時間21分．[Code1](#Code1).
  * 基本は102. Binary Tree Level Order Traversalと同じ．
  * bool型の変数`traverse_from_left`を持っておいて，whileループを通過するたびに切り替える．
    * `traverse_from_left`に応じて左からtraverseするか右からtraverseするか決める．
    * `deque`を用いて`push_back` & `pop_back`と`push_front` & `pop_front`を切り替えればいけそう
  * `deque`->`vector`の変換のところで`traverse_from_left`を用いても良いと思ったが，`vector`は`push_back`の方が`push_front`より早い（前者はO(1), 後者はO(len(vector))）なので一旦不採用．
    * ただ，ここのボトルネックが小さいなら可読性的にこちらを用いても良い．
    * (Step2で後述) `reverse`を用いればこちらの方針でも`push_back`のみで完結する．
  * 書いてみて気づいたが，使うのは`push_back`と`pop_back`のみで良い
    * `push_back`していったdequeに対して`pop_back`していけば勝手にジグザグになる．
    * ただし，各nodeのleft, rightをdequeに入れるときにleft -> rightの順で入れるか，right -> leftの順で入れるかを決定するのに`traverse_from_left`は必要．
    * これで完成したのが[Code1](#Code1)．

### Code1
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) {
            return result;
        }

        std::deque<TreeNode*> frontiers;
        std::deque<TreeNode*> next_frontiers;
        next_frontiers.push_back(root);
        bool traverse_from_left = false;
        while (!next_frontiers.empty()) {
            frontiers = std::move(next_frontiers);
            next_frontiers.clear();
            result.push_back(GetVectorFromDeque(frontiers));

            while (!frontiers.empty()) {
                TreeNode* node = frontiers.back();
                frontiers.pop_back();

                if (traverse_from_left) {
                    for (TreeNode* child : {node->left, node->right}) {
                        if (child) {
                            next_frontiers.push_back(child);
                        }
                    }
                } else {
                    for (TreeNode* child : {node->right, node->left}) {
                        if (child) {
                            next_frontiers.push_back(child);
                        }
                    }
                }
            }

            traverse_from_left = !traverse_from_left;
        }
        return result;
    }

private:
    vector<int> GetVectorFromDeque(std::deque<TreeNode*> nodes_deque) {
        vector<int> nodes_vector;
        for (TreeNode* node : nodes_deque) {
            nodes_vector.push_back(node->val);
        }
        return nodes_vector;
    }
};
```
# Step 2
* [他の人のPRを見る](https://github.com/5103246/LeetCode_Arai60/blob/103-binary-tree-zigzag-level-order-traversal/103-binary-tree-zigzag-level-order-traversal/103-binary-tree-zigzag-level-order-traversal.md)
* DFS，BFSの一時コンテナにとりあえず普通に追加した後，`vector`を得る段階で反転させる方法．
  * [`std::reverse`](https://cpprefjp.github.io/reference/algorithm/reverse.html)の利用．[Code2](#Code2)
    * Step1で言及した，`deque`から`vector`への変換のところでreverseを利用
    * `GetVectorFromDeque()`の計算量は，`len(deque) / 2`回 swap するのでO(len(deque))だけ増える．
    * しかし元々O(len(deque))なので結局定数倍のみ．．
  * [`std::view::reverse`](https://cpprefjp.github.io/reference/ranges/reverse_view.html)の利用．[Code3](#Code3)
    * 実体自体を並び替えないので`std::reverse`より早い．
  * 逆イテレータの利用もあるが，for文の中身が煩雑になるだけなので`std::view::reverse`で良い．
 
### Code2
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) {
            return result;
        }

        std::deque<TreeNode*> frontiers;
        std::deque<TreeNode*> next_frontiers;
        next_frontiers.push_back(root);
        bool traverse_from_left = true;
        while (!next_frontiers.empty()) {
            frontiers = std::move(next_frontiers);
            next_frontiers.clear();
            result.push_back(GetVectorFromDeque(frontiers, traverse_from_left));

            while (!frontiers.empty()) {
                TreeNode* node = frontiers.front();
                frontiers.pop_front();
                for (TreeNode* child : {node->left, node->right}) {
                    if (child) {
                        next_frontiers.push_back(child);
                    }
                }
            }

            traverse_from_left = !traverse_from_left;
        }
        return result;
    }

private:
    vector<int> GetVectorFromDeque(std::deque<TreeNode*> nodes_deque, bool traverse_from_left) {
        vector<int> nodes_vector;
        if (!traverse_from_left) {
            std::reverse(nodes_deque.begin(), nodes_deque.end());
        }
        for (TreeNode* node : nodes_deque) {
            nodes_vector.push_back(node->val);
        }
        return nodes_vector;
    }
};
```

### Code3
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) {
            return result;
        }

        std::deque<TreeNode*> frontiers;
        std::deque<TreeNode*> next_frontiers;
        next_frontiers.push_back(root);
        bool traverse_from_left = true;
        while (!next_frontiers.empty()) {
            frontiers = std::move(next_frontiers);
            next_frontiers.clear();
            result.push_back(GetVectorFromDeque(frontiers, traverse_from_left));

            while (!frontiers.empty()) {
                TreeNode* node = frontiers.front();
                frontiers.pop_front();
                for (TreeNode* child : {node->left, node->right}) {
                    if (child) {
                        next_frontiers.push_back(child);
                    }
                }
            }

            traverse_from_left = !traverse_from_left;
        }
        return result;
    }

private:
    vector<int> GetVectorFromDeque(std::deque<TreeNode*> nodes_deque, bool traverse_from_left) {
        vector<int> nodes_vector;

        if (traverse_from_left) {
            for (TreeNode* node : nodes_deque) {
                nodes_vector.push_back(node->val);
            }
            return nodes_vector;
        }

        // traverse_from_left == false --> 逆順に走査
        for (TreeNode* node : std::views::reverse(nodes_deque)) {
            nodes_vector.push_back(node->val);
        }
        return nodes_vector;
    }
};
```

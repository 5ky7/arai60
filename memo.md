# Step 1
* 普通に解いてみる．所要時間21分．[Code1](#Code1).
  * 基本は102. Binary Tree Level Order Traversalと同じ．
  * bool型の変数`traverse_from_left`を持っておいて，whileループを通過するたびに切り替える．
    * `traverse_from_left`に応じて左からtraverseするか右からtraverseするか決める．
    * `deque`を用いて`push_back` & `pop_back`と`push_front` & `pop_front`を切り替えればいけそう
  * `deque`->`vector`の変換のところで`traverse_from_left`を用いても良いと思ったが，`vector`は`push_back`の方が`push_front`より早い（前者はO(1), 後者はO(len(vector))）なので一旦不採用．
    * ただ，ここのボトルネックが小さいなら可読性的にこちらを用いても良い．
  * 書いてみて気づいたが，使うのは`push_back`と`pop_back`のみで良い
    * `push_back`していったdequeに対して`pop_back`していけば勝手にジグザグになる．
    * ただし，各nodeのleft, rightをdequeに入れるときにleft -> rightの順で入れるか，right -> leftの順で入れるかを決定するのに`traverse_from_left`は必要．
    * これで完成したのが[Code1](#Code1)．

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
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

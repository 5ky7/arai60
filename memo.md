[105. Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/description/)
# Step 1
* 少し考えてみたが，コードにできなかった．以下回答を見るまでの思考過程．
  * 手元で5個のノードからなる例，3個のノードからなる例を考えてみた
  * preorder, inorderの要素を指すイテレータを用意する．
  * it_preorderの値をもつノード`node`をとりあえず作る．
  * it_inorderの値とit_preorderの値が違うなら，`node->left`を構築する．適宜イテレータを進める．
  * it_inorderの値とit_preorderの値が同じになったら（あるいは同じだったら）`node->right`を構築していく．
  * このような考えのもとで再帰的に書こうとしていたが，`node->left`を再帰的に構築して行った後にどうやって元の`node`に戻ってくるのかがよくわからなくなってしまった．
  * また，`node->left`がある場合とない場合とで，元の`node`へ戻るところの処理が異なる気がしていたが，ここがよくわからなかった．
    * 例えば`preorder = [3, 9, 7, 8, 20, 7]`, `inorder = [7, 9, 8, 3, 20, 7]`という構造を考える．
    * `preorder[3] (== 8)`までの構築の手順を見ると，indexと上記説明のイテレータの対応に注意すると，
      * `TreeNode(preorder[0])`を作る．`preorder[0] != inorder[0]`なのでleftを見る．it_preorderを進める．
      * `TreeNode(preorder[1])`を作る．`preorder[1] != inorder[0]`なのでleftを見る．it_preorderを進める．
      * `TreeNode(preorder[2])`を作る．`preorder[2] == inorder[0]`なのでit_inorderを進める．`TreeNode(inorder[1])`となるノードまで戻って，そのrightを見る．it_preorderを進める．
      * `TreeNode(preorder[3])`を作る．．．
    * という流れになる．しかしこれが最後の方，つまり`preorder[4]`からの手順を見ると，
      * `TreeNode(preorder[4])`を作る．`preorder[4] == inorder[4]`なのでit_inorderを進める，`TreeNode(inorder[5]`となるノードに*戻る必要はなく（というか，戻れず）*，今のノードからrightを見る．it_preorderを進める．
    * ということになる，つまり，一旦どこかのノードでright側に潜った場合は「戻る」操作が入るが，潜らない場合は「戻る」操作を入れずにleftに移ることになる．
    * ここの実装がややこしくなってしまって，時間もだいぶ経っていたので，回答を見た．
* 回答を見ると，inorderが，とあるノードから見たleft, rightのサブツリーの情報を与えていることに注目し，再帰的に解いていた．
  * つまり，`inorder = [7, 9, 8, 3, 20, 7]`に対して，
    * ノード`[3]`のleftには`[7, 9, 8]`が，rightには`[20, 7]`が含まれている．
    * ノード`[9]`のleftには`[7]`が，rightには`[8]`が含まれている．
    * ノード`[20]`のleftには`[]`（空）が，rightには`[7]`が含まれている．
  * この構造をうまく再帰的に用いていた．この考え方を参考に実装したのが[Code1](#Code1)．
    * とあるノードを見た時，そのノードのleftサブツリー，rightサブツリーに用いるノードが含まれているinorderの範囲(`left_inorder, right_inorder`で指定）をそれぞれ再帰関数に与えてやる．
    * base caseとして範囲で指定される領域が空になったらnullptrを返せば良い，という方針．
 
### Code1
```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        map<int, int> values_to_inorder_indices;
        for (int i = 0; i < inorder.size(); ++i) {
            values_to_inorder_indices[inorder[i]] = i;
        }

        int index_root_val = 0;
        return buildSubTree(preorder,
                            inorder,
                            values_to_inorder_indices,
                            0,
                            inorder.size() - 1,
                            index_root_val);
    }

private:
    TreeNode* buildSubTree(const vector<int>& preorder,
                           const vector<int>& inorder,
                           map<int, int>& values_to_inorder_indices,
                           int left_inorder,
                           int right_inorder,
                           int& index_root_val) {
        if (left_inorder > right_inorder) {
            --index_root_val;
            return nullptr;
        }

        int root_val = preorder[index_root_val];
        TreeNode* new_node = new TreeNode(root_val);
        new_node->left = buildSubTree(preorder, 
                                      inorder,
                                      values_to_inorder_indices,
                                      left_inorder,
                                      values_to_inorder_indices[root_val] - 1,
                                      ++index_root_val);
        new_node->right = buildSubTree(preorder, 
                                      inorder,
                                      values_to_inorder_indices,
                                      values_to_inorder_indices[root_val] + 1,
                                      right_inorder,
                                      ++index_root_val);
        
        return new_node;
    }
};
```

# Step 2
* [こちら](https://github.com/kazukiii/leetcode/pull/30/files#diff-06efd0304bbfd575bfe41bfea0314658b1edecf67e00e23997ed8354f2e210d8)を参考に，`preorder`,`inorder`を構築し直すことで`buildTree()`を直接再帰関数化しようとしたが，`preorder`などを作り直すたびに空間計算量が無駄に嵩むのでspanを用いたのが[Code2](#Code2)．
* [preorderの順に構築する方法](https://github.com/kazukiii/leetcode/pull/30#discussion_r1821506570)を参考にしたのが[Code3](#Code3)．Step1で中断した考え方（「親に戻る」，より正確には「親を特定する」方針）に近い．
  * 子が確定していないノードからなるスタック`nodes_may_have_a_child`を用意して親がその中にいるはず，という考え方をする．
  * 子が確定したら（つまりleftとrightを見終わったら）スタックから取り出す．
* [inorderの順で構築していく](https://github.com/kazukiii/leetcode/pull/30/files/ab2dbb435e41621f4b7e17de01e8afcf984af434#r1821628634)方法を参考に実装してみたのが[Code4](#Code4)だが，理解しきれていない．
  * `gather_descendants`の`while`ループ内の動作で，「`node->left`以下のノードのrightが全て確定する」と言える理由がわかっていない．
    * whileループを繰り返すに従って，`unchained_node`はどんどん祖先側にいく．同時に`unchained_node->right = child`の瞬間に`unchained_node`はrightも確定する．
    * しかし本当に木構造の左下のノードから順にrightが確定していくと言えるのか？ここが理解しきれていない．



### Code2
```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        span<const int> preorder_span(preorder);
        span<const int> inorder_span(inorder);
        return buildTreeHelper(preorder_span, inorder_span);
    }

private:
    TreeNode* buildTreeHelper(span<const int> preorder, span<const int> inorder) {
        if (preorder.empty() || inorder.empty()) {
            return nullptr;
        }

        int root_val = preorder[0];
        TreeNode* root = new TreeNode(root_val);

        auto it_root_val_inorder = std::find(inorder.begin(), inorder.end(), root_val);
        int num_nodes_left_subtree = distance(inorder.begin(), it_root_val_inorder);

        span<const int> inorder_left_subtree = inorder.subspan(0, num_nodes_left_subtree);
        span<const int> inorder_right_subtree = inorder.subspan(num_nodes_left_subtree + 1);

        span<const int> preorder_left_subtree = preorder.subspan(1, num_nodes_left_subtree);
        span<const int> preorder_right_subtree = preorder.subspan(num_nodes_left_subtree + 1);

        root->left = buildTreeHelper(preorder_left_subtree, inorder_left_subtree);
        root->right = buildTreeHelper(preorder_right_subtree, inorder_right_subtree);
        
        return root;
    }
};
```

### Code3
```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        map<int, int> values_to_index_inorder;
        for (int i = 0; i < inorder.size(); ++i) {
            values_to_index_inorder[inorder[i]] = i;
        }

        TreeNode dummy;
        stack<tuple<TreeNode*, int, int>> nodes_may_have_a_child;
        // `nodes_may_have_a_child` contains all nodes that their children have not confirmed.
        // the second and third values represent the range of index in `inorder` that corresponds to the right subtree of the node.
        nodes_may_have_a_child.emplace(&dummy, numeric_limits<int>::max(), numeric_limits<int>::max());
        for (int node_val : preorder) {
            TreeNode* node = new TreeNode(node_val); 
            // The problem is: which is the parent of `node`?
            // Case1 : `node` is the child of the top node of the stack.
            //  Case1-1 : `node` is the *left* child of the top node of the stack.
            //  Case1-2 : `node` is the *right* child of the top node of the stack.
            // Case2 : `node`is not the child of the top node of the stack.
            int node_index_inorder = values_to_index_inorder[node_val];
            auto [parent, first_right_subtree, last_right_subtree] = nodes_may_have_a_child.top();
            // `parent` is the top node of the stack.
            if (node_index_inorder < first_right_subtree) {
                // Case1-1
                parent->left = node;
                nodes_may_have_a_child.emplace(node, node_index_inorder + 1, first_right_subtree);
                continue;
            }
            while (true) {
                auto [parent, first_right_subtree, last_right_subtree] = nodes_may_have_a_child.top();
                if (node_index_inorder < last_right_subtree) {
                    // Case1-2
                    parent->right = node;
                    nodes_may_have_a_child.pop(); // Children of `parent` have confirmed. 
                    nodes_may_have_a_child.emplace(node, node_index_inorder, last_right_subtree);
                    break;
                }
                // Case2 : the parent of `node` is the node added to the stack before `parent`
                // So pop the stack and check the next (that is, older) `parent` by while loop.
                nodes_may_have_a_child.pop(); // Children of `parent` have confirmed. 
            }
        }

        return dummy.left;
    }
};
```

### Code4
```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        map<int, int> values_to_position_preorder;
        for (int i = 0; i < preorder.size();++i) {
            values_to_position_preorder[preorder[i]] = i;
        }

        stack<TreeNode*> nodes_not_finalized_right;
        auto gather_descendants = [&](int node_position) {
            // gather_descendantsの呼び出しによって，node->left以下のノードのrightが全て確定しなければならない．
            // まだ確定していないnode->left以下のノードはnodes_not_finalized_rightに入っている．
            TreeNode* child = nullptr;
            while (!nodes_not_finalized_right.empty()) {
                TreeNode* unchained_node = nodes_not_finalized_right.top();
                if (values_to_position_preorder[unchained_node->val] < node_position) {
                    break;
                }
                nodes_not_finalized_right.pop();
                unchained_node->right = child;
                child = unchained_node;
            }
            return child;
        };
        for (int node_val : inorder) {
            TreeNode* node = new TreeNode(node_val);
            int node_position = values_to_position_preorder[node->val];
            node->left = gather_descendants(node_position); 
            nodes_not_finalized_right.emplace(node);
        }
        return gather_descendants(numeric_limits<int>::min());
    }
};
```

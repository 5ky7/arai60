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

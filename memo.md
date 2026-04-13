# Step 1
* 考えたがわからなかったので過去の提出を確認
  * あるnodeに対して，node->leftにはnode->valより小さい値のみ，node->rightにはnode->valより大きい値のみが入っている
  * これって2分探索の考え方に似ている．numsが昇順であることとピッタリ整合
  * 2分探索同様，Helper関数にnumsとindex範囲の下限と上限を渡しておけば良い．([Code1](Code1))
### Code1
```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return Helper(nums, 0, nums.size() - 1);
    }
private:
    TreeNode* Helper(const vector<int>& nums, int left, int right) {
        if (left > right) {
            return nullptr;
        }

        int center = (left + right) / 2;
        TreeNode* root = new TreeNode(nums[center]);
        root->left = Helper(nums, left, center - 1);
        root->right = Helper(nums, center + 1, right);

        return root;
    }
};
```
# Step 2
* Step1から時間が空いたので解き直してみる．[Code2](#Code2)．所要時間5分．
  * Step1との違いは変数名，二分探索の領域指定が閉区間か半開区間か．
* 例によって例の如く再帰をループで書き直す．[Code3](#Code3)．所要時間40分ほど．
  * こんがらがった．
  * 半開区間でやると範囲外アクセスを引き起こしてしまい，構造上不可避な気がしたので閉区間でやった．
  * while文の中でノードを作る=newするタイミングをどうするかで2パターンある
    * frontiersにとりあえずlower, upper, nullptrのポインタを入れといて，そのポインタの番が来たら，ノード（ = 「ポインタの中身の中身」）を，lowerとupperから作る（今回のコードで採用）
    * frontiersに入れる前にlowerとupperからノードを作る．その後，その対応するlowerとupperとノードのポインタのポインタをfrontiersに入れる
  * と思ったが，上のやつ後者の方は[メモリリークの危険がある](https://github.com/irohafternoon/LeetCode/pull/27/files/6d5bfca367a0487c71117a848d66ffd4c6036136#r2060734803)
    * とりあえず作ってからnullptrに置き換えるとnewしたやつがdeleteされずに残る．よってnullptrに置き換えるまえにデリートする．
  * `Info`に関しては，いい名前が思い浮かばなかった．どうせ構造化束縛で取り出すのでメンバ変数名から内容物がわかればあんまり長くする必要もないとも思ったので，シンプルに済ませた．
  * `*ptr_to_node`がたくさん登場したのでこれを`node`でおいたが，`node`は`TreeNode*&`であることに注意
    * `TreeNode* node = *ptr_to_node`とすると`*ptr_to_node`のコピーが作成されて`node`に入る．
    * `node`を書き換えても`*ptr_to_node`は変わらない．


### Code2
```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return Helper(nums, 0, nums.size());
    }
private:
    TreeNode* Helper(const vector<int>& nums, const int left, const int right) {
        if (left >= right) {
            return nullptr;
        }

        int center = (left + right) / 2;
        TreeNode* new_root = new TreeNode(nums[center]);
        new_root->left = Helper(nums, left, center);
        new_root->right = Helper(nums, center + 1, right);
        return new_root;
    }
};
```

### Code3
```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.size() == 0) {
            return nullptr;
        }

        std::deque<Info> frontiers;
        int center = (nums.size() - 1) / 2;
        TreeNode* new_root = nullptr;
        frontiers.push_back({0, int(nums.size() - 1), &new_root});
        while (!frontiers.empty()) {
            auto [lower, upper, ptr_to_node] = frontiers.front();
            frontiers.pop_front();
            TreeNode*& node = *ptr_to_node;
            if (lower > upper) {
                node = nullptr;
                continue;
            }
            int center = (lower + upper) / 2;
            node = new TreeNode(nums[center]);

            frontiers.push_back({lower, center - 1, &(node->left)});
            frontiers.push_back({center + 1, upper, &(node->right)});
        }

        return new_root;
    }
private:
    struct Info {
        int lower;
        int upper;
        TreeNode** ptr_to_node;
    };
};
```

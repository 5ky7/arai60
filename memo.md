# Step 1
* いつか解いた時のものが[Code1](#Code1)．
* [Code1](#Code1)から時間が経ったので再度解き直してみる．[Code2](#Code2)．所要時間6分．
  * とあるnodeに対して，node->leftかnode->rightのどっちかまでの和が`targetSum - node->val`になれば良い．
  * 初めはhasPathSumの中で`node == nullptr`の時を除外して，再帰用ヘルパー関数の中では登場するnodeが全てnullptrでないようにしようとしたが，そうすると再帰関数内のreturn文のところで場合わけをする必要が出てきて面倒（つまりnode->leftがnullptrの時に再帰関数にnode->leftを渡せない）
  * そこで`node == nullptr`のケースも再帰関数内に含めて考えた．そうすると再帰用ヘルパー関数を別に用意する必要も無くなった．
  * 結構[Code1](#Code1)と[Code2](#Code2)で条件の捉え方が違う．
    * 個人的には[Code2](#Code2)の方がスッキリしていると思う．
      * if文の中身がシンプルなのと，「左右の子供どっちかで条件を満たせてればOK」という考え方とコードの距離が近い．

### Code1
```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) return false;
        return Traverse(root, targetSum, 0);
    }
private:
    bool Traverse(TreeNode* node, const int target, int sum) {
        sum += node->val;
        if (!node->left && !node->right) {
            return sum == target;
        }

        if (node->left && Traverse(node->left, target, sum)) {
            return true;
        }
        if (node->right && Traverse(node->right, target, sum)) {
            return true;
        }

        return false;
    }
};
```
### Code2
```
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return false;
        }
        if (!root->left && !root->right) {
            return targetSum == root->val;
        }
        
        int nextTargetSum = targetSum - root->val;
        return hasPathSum(root->left, nextTargetSum) || hasPathSum(root->right, nextTargetSum);
    }
};
```

# Step 2
* ループで書き直してみる．[Code3](#Code3)．所要時間10分．
  * BFSよりもDFSの方が早そう，と思ったがそうでもないか？
    * 無駄な探索をする可能性があるのでそうでもなさそう．
  * とりあえずDFSで書いた．
* [スタック（キュー）に入れる前にnullptrチェックをするか，とりあえず入れてから処理する前にnullptrチェックをするか](https://github.com/5103246/LeetCode_Arai60/pull/24)の観点を忘れていた．後者でもやってみる．[Code4](#Code4)．
  * こうすると`hasPathSum`本体の最初の`if (!root) {...`の分岐も不要に．

### Code3
```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return false;
        }

        std::deque<NodeAndTarget> frontiers;
        frontiers.push_back({root, targetSum});
        while (!frontiers.empty()) {
            auto [node, target] = frontiers.back();
            frontiers.pop_back();
            int new_target = target - node->val;
            if (!node->left && !node->right) {
                // 葉に到達
                if (new_target == 0) {
                    return true;
                }
            }
            for (TreeNode* child : {node->left, node->right}) {
                if (child) {
                    frontiers.push_back({child, new_target});
                }
            }
        }
        return false;
    }
private:
    struct NodeAndTarget {
        TreeNode* node;
        int target;
    };
};
```

### Code4
```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        std::deque<NodeAndTarget> frontiers;
        frontiers.push_back({root, targetSum});
        while (!frontiers.empty()) {
            auto [node, target] = frontiers.back();
            frontiers.pop_back();
            if (!node) {
                continue;
            }
            int new_target = target - node->val;
            if (!node->left && !node->right && new_target == 0) {
                return true;
            }
            frontiers.push_back({node->left, new_target});
            frontiers.push_back({node->right, new_target});
        }
        return false;
    }
private:
    struct NodeAndTarget {
        TreeNode* node;
        int target;
    };
};
```

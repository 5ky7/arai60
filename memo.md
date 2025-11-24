# Step 1
* まず思ったのが対称性を活かすか，崩すか．
  * root1とroot2で同じ扱いをするには新たにnewでノード用のメモリを確保していく．
  * 対称性を崩して良いならroot1を破壊的に変更し，最終的にroot1を返す．
* 前者で書いてみた([Code1](#Code1)).理由は：
  * 対象な扱いをしてコードが綺麗になる感じもしない（むしろ崩した方が書きやすそう）．
  * ポインタを返すのも良くないし，nullで返せないのが面倒くさい．

### Code1
```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return Helper(root1, root2);
    }
private:
    TreeNode* Helper(TreeNode* root1, TreeNode* root2) {
        if (root1 && root2) {
            root1->val += root2->val;
            root1->left = Helper(root1->left, root2->left);
            root1->right = Helper(root1->right, root2->right);
            return root1;
        }
        if (root1) {
            return root1;
        }
        if (root2) {
            return root2;
        }
        return nullptr;
    }
};
```

# Step 2

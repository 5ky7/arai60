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
* 例によってStep1から時間があいてしまったので解き直す([Code3](#Code3))．
  * 所要時間7分．
  * 書いてみて，処理の優先順位がStep1と違うことに気づいた．
    * Step2では「簡単に処理できるケースを先に処理する」というイメージになっている．
* 非破壊的に書いてみる([Code4](#Code4))．
  * `return nullptr`を使いたいので再帰関数の返り値はあくまで`TreeNode*`型
  * `node->val`を足し合わせる必要がないなら元のnodeをそのまま使う
  * 足し合わせる必要があるときは`new`で関数を抜けてもdeleteされないようにヒープ上に確保してこれをreturnする．
* [Code4](#Code4)の「元のnodeをそのまま使う」のところは，「ここに分岐で至った時点でそれ以上書き換えることがない（＝元のノードは非破壊）」という認識をしていたが，合成後のノード全てを新しく生成するパターンの実装が多くみられたのでそれもやってみる([Code5](#Code5))
 * `const`とか`constexpr`とかも含めて型になるので，`const TreeNode*`に`TreeNode*`を入れようとするとキャストが起こることに注意．
 
### Code3
```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return MergeTreesHelper(root1, root2);
    }
private:
    TreeNode* MergeTreesHelper(TreeNode* root1, TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }
        root1->val += root2->val;
        root1->left = MergeTreesHelper(root1->left, root2->left);
        root1->right = MergeTreesHelper(root1->right, root2->right);
        return root1;
    }
};
```
### Code4
```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return MergeTreesHelper(root1, root2);
    }
private:
    TreeNode* MergeTreesHelper(const TreeNode* root1, const TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }

        // root1もroot2も存在するなら新しいrootを作成して返す
        TreeNode* new_root = new TreeNode(root1->val + root2->val);
        new_root->left = MergeTreesHelper(root1->left, root2->left);
        new_root->right = MergeTreesHelper(root1->right, root2->right);
        return new_root;
    }
};
```

### Code5
```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return MergeTreesHelper(root1, root2);
    }
private:
    const TreeNode dummy_ = TreeNode();

    TreeNode* MergeTreesHelper(const TreeNode* root1, const TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (!root1) {
            root1 = &dummy_;
        }
        if (!root2) {
            root2 = &dummy_;
        }

        TreeNode* new_root = new TreeNode(root1->val + root2->val);
        new_root->left = MergeTreesHelper(root1->left, root2->left);
        new_root->right = MergeTreesHelper(root1->right, root2->right);
        return new_root;
    }
};
```

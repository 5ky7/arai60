# Step 1
* Maximum Depthのmaxをminにしただけ，と思って書いたがWA（[Code1](#Code1）
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
* 再帰をループで書いてみる

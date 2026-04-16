[779. K-th Symbol in Grammar](https://leetcode.com/problems/k-th-symbol-in-grammar/)
# Step 1
とりあえず解いてみたのが[Code1](#Code1)．

#### Code1
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        int num_leaves = static_cast<int>(std::pow(2, n - 1));
        if (n <= 0 || k <= 0 || k > num_leaves) {
            throw "Invalid Argument(s)";
        }
        return KthGrammerHelper(n - 1, k - 1, 0);
    }

private:
    int KthGrammerHelper(int depth, int index, int origin) {
        // depth: depth of the table(0-indexed)
        // index: position of object number(0-indexed)
        if (depth == 0) {
            return origin;
        }
        int num_leaves = static_cast<int>(std::pow(2, depth));
        int new_origin = index < num_leaves / 2 ? origin : 1 - origin;
        int new_index = index % (num_leaves / 2);
        return KthGrammerHelper(depth - 1, new_index, new_origin);
    }
};
```

---
数学的に考えると面白く解けたことを思い出して考えてみる．

0から始まって01,0110, ... と数字が生まれていくが，これを木構造で考える．つまり以下のような木を考える
```
# n = 2の時

        0
    0       1
  0   1   1   0
```
n段目，つまり下端の数字（木構造でいう葉）について，k番目の数字（`ans`とする）が何か聞かれているわけだが，これはkが葉の中で左半分にあるか，右半分にあるかで考えられる．つまり根である0から見て，左側の部分木に含まれているか，右側の部分木に含まれているか，を考え，その部分木について同じ問題を再帰的に考えることができる．具体的にはn段目の葉の数を`K`として，
- kが葉のうち左半分に含まれていれば，0を根とする木の`n-1`段目の`k`番目の数字が`ans`
- kが葉のうち右半分に含まれていれば，（0ではなく！）1を根とする木の`n-1`段目の`k - K`番目の数字が`ans`

これは$`2^i \; (i = n - 1, n - 2, \ldots, 0)`$を`k`から引けるだけ引いていき，引ける時に根の0，1を反転するということに他ならない．最終的に頂点が根の位置にきた時，その数字こそが`ans`である．
初め根は0なので，偶数回引けたら`ans = 0`，奇数回引けたら`ans = 1`になるということだ．そして「$`2^i \; (i = n - 1, n - 2, \ldots, 0)`$を`k`から引けるだけ引く」というのは，`k`の2進展開を計算していることと同義なので，2進展開時の1の数を数えれば良い．ただし，正しくは上の議論を`k`ではなく`k - 1`について行う必要があることに注意する．[Code2](#Code2)．
#### Code2
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        std::bitset<29> k_bit(k - 1);
        if (k_bit.count() % 2 == 0) {
            return 0;
        } else {
            return 1;
        }
    }
};
```

---
かなり以前に一度解いた時のものが[PastCode](#PastCode)．

#### PastCode
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        int column = k;
        map<int, int> row_to_column;
        for (int i = n; i > 0; --i) {
            row_to_column[i] = column;
            column = (column + 1) / 2;
        }

        int digit = 0;
        for (int i = 1; i <= n; ++i) {
            digit = (row_to_column[i] % 2 == 0) ? 1 - digit : digit;
        }
        return digit;
    }
};
```
```cpp
// 数学的考察...kから何回leaf_num / 2を引き算するか？-->kの二進展開に基づく．
class Solution {
public:
    int kthGrammar(int n, int k) {
        bitset<29> k_binary(k - 1);
        if (k_binary.count() % 2 == 0) {
            return 0;
        } else {
            return 1;
        }
    }
};
```
```cpp
// 再帰的二分木探索
class Solution {
private:
    int Helper(int n, int k, int root_val) {
        if (n == 1) {
            return root_val;
        }

        int num_leaf = std::pow(2, n - 1);
        if (k > num_leaf / 2) {
            return Helper(n - 1, k - num_leaf / 2, 1 - root_val);
        } else {
            return Helper(n - 1, k, root_val);
        }
    }
public:
    int kthGrammar(int n, int k) {
        return Helper(n, k, 0);
    }
};
```
```cpp
// simple recursion (comp:O(n), spacial:O(n))
class Solution {
public:
    int kthGrammar(int n, int k) {
        if (n == 1) {
            return 0;
        }

        if (k % 2 == 0) {
            return 1 - kthGrammar(n - 1, k / 2);
        } else {
            return kthGrammar(n - 1, (k + 1) / 2);
        }
    }
};
```
```cpp
// backtrack (comp:O(n), spacial:O(n))
class Solution {
public:
    int kthGrammar(int n, int k) {
        vector<int> index_root_to_leaf(n);
        int column = k;
        for (int i = n - 1; 0 <= i; --i) {
            index_root_to_leaf[i] = column;
            column = (column + 1) / 2;
        }

        int num_leaf = 0;
        for (int i = 0; i < n; ++i) {
            if (index_root_to_leaf[i] % 2 == 0) num_leaf = 1 - num_leaf; 
        }
        return num_leaf;
    }
};
```

# Step 2

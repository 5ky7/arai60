[62. Unique Paths](https://leetcode.com/problems/unique-paths/)
# Step 1
いつだか解いたものが[Code1](#Code1)．

時間が経ったので解き直してみる：[Code2](#Coed2)．5分ほど．
単純に考え方は知っていた．実装上の問題を確認すると，
> The test cases are generated so that the answer will be less than or equal to $`2 \times 10^9`$.

とのことなのでC++のintの範囲が$`[-2^{31}, 2^{31} - 1]`$であることと合わせて，`int`で事足りる．時間・空間計算量はどちらも$`O(n^2)`$．

### Code1
```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> num_paths(m + 1, vector<int>(n + 1, 0));
        num_paths[1][1] = 1;
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (i == 1 && j == 1) {
                    continue;
                }
                num_paths[i][j] = num_paths[i - 1][j] + num_paths[i][j - 1];
            }
        }

        return num_paths[m][n];
    }
};
```

### Code2
```cpp
#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> num_paths(m + 1, vector<int>(n + 1, 0));
        num_paths[1][1] = 1;
        for (int row = 1; row <= m; ++row) {
            for (int col = 1; col <= n; ++col) {
                num_paths[row][col] += num_paths[row - 1][col] + num_paths[row][col - 1];
            }
        }
        return num_paths[m][n];
    }
};
```

# Step 2
　ボトムアップを書いたらトップダウンを書くのが礼儀というものなので書いたのが[Code3](#Code3)．0-indexか1-indexかでミスをしてASanに怒られたので修正した．
- ヘルパー関数の引数を0-indexにすれば，ヘルパー関数内で引数をそのまま`num_paths`のindexとして扱えるが，ヘルパー関数に渡すのが`m-1, n-1`になって少し気持ち悪い．
- ヘルパー関数の引数を1-indexにすれば，ヘルパー関数にそのまま`m, n`を渡せるが，関数内で`num_paths`の添え字に対応させるため`m-1, n-1`で扱う必要がある．

今回は，「気持ち悪さは見えないところに」の気持ちで後者にした．時間・空間計算量はどちらも$`O(n^2)`$．

### Code3
```cpp
#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> num_paths(m, vector<int>(n, -1));
        return uniquePathsHelper(m, n, num_paths);
    }

private:
    int uniquePathsHelper(const int m, const int n, vector<vector<int>>& num_paths) {
        // m,n : [1, 100]
        int row = m - 1;
        int col = n - 1;
        if (row == 0 || col == 0) {
            return 1;
        }
        if (num_paths[row - 1][col] == -1) {
            num_paths[row - 1][col] = uniquePathsHelper(m - 1, n, num_paths);
        }
        if (num_paths[row][col - 1] == -1) {
            num_paths[row][col - 1] = uniquePathsHelper(m, n - 1, num_paths);
        }
        return num_paths[row - 1][col] + num_paths[row][col - 1];
    }
};
```

---
　再帰はメモ化しないと計算量がフィボナッチ数的に増えるというパターンが多いが，今回は

$$ S_{i,j} = S_{i-1, j} + S_{i, j-1} \quad (i,j>0) $$
$$ S_{0,j} = S_{i, 0} = 1 $$

ということで少しフィボナッチとは違う形．一瞬解こうとしたけど冷静に考えたら「右」を$`m-1`$文字，と「下」を$`n-1`$文字使って作れる文字列の種類と等しいから

$$ \binom{m+n-2}{n-1} $$

で，結局フィボナッチと同じく指数オーダー．

　計算でわかるならそのまま定数時間のコードが書けるじゃん，と思ったがC++のstdとかに2項計数とか階乗とかあったっけ？と思ったらやっぱりなかった．というか，stdにあっても関数の実行時間があるから定数時間ではないという当たり前のことに気づく．まあ自前で実装するよりかは多少の高速化・精度保持の工夫はされてそうだが．

---
　[これ](https://discord.com/channels/1084280443945353267/1339428945845555252/1360645783300341760)動くらしくて驚き．ということは`m * n`でなく`1 * n`のデータにした[Code4](#Code4)が動くということで，実際動いた．確かに**更新が一方通行**だと，一旦行を進めたら前の行を参照することはないから，当然か．Maximum Subarrayで経験した[Kadaneの空間計算量削減](https://github.com/5ky7/arai60/pull/33/changes#diff-0c860cd754249868513e4f9054206317fa33d0f548fc3896ac2b3e11822fd852R13)と同じ仕組み．

### Code4
```python
class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        num_possible = [0] * n
        for row in range(m):
            for column in range(n):
                if row == 0 or column == 0:
                    num_possible[column] = 1
                    continue
                num_possible[column] = num_possible[column] + num_possible[column - 1]
        return num_possible[n - 1]
```

---
以下個人的メモ
- [Pythonでの掛け算でのリスト作成](https://discord.com/channels/1084280443945353267/1339428945845555252/1360645783300341760)．
  - 基本的には同じものを複製するだけである．
  - 要素がintとかでimmutableなら作り直しされ問題ないが，要素がリストなどmutableだと1箇所変えただけで全て置き換わるので注意．




# Step 1
いつだか解いたものが[Code1](#Code1)．

時間が経ったので解き直してみる．[Code2](#Code2)を書いたら`[[1]]`という入力に対してWA．こういうエッジケースをちゃんと事前に気づけるようにしたい．ということで例外処理を追加したのが[Code3](#Code3)．
テストケース例を考えてみる（`入力` : 想定出力）
- `[[1]]` : 0
- `[[0,1], [0,0]]` : 1
- `[[0,0], [0,1]]` : 0
- `[[0,1,0],[0,0,0],[0,0,0]]` : 3
- `[[0,0,0],[1,0,0],[0,0,0]]` : 3
- `[[0,0,0],[0,0,1],[0,0,0]]` : 3
- `[[0,0,0],[0,0,0],[0,1,0]]` : 3
- `[[0,0,0],[0,1,0],[0,0,0]]` : 2


#### Code1
```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();

        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1]) return 0;

        vector<vector<int>> grid_n_paths(m + 1, vector<int>(n + 1, 0));
        grid_n_paths[1][1] = 1;

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (obstacleGrid[i - 1][j - 1]) continue;
                grid_n_paths[i][j] += grid_n_paths[i][j - 1] + grid_n_paths[i - 1][j];
            }
        }
        return grid_n_paths[m][n];
    }
};
```

#### Code2
```cpp
#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        vector<vector<int>> num_paths(m + 1, vector<int>(n + 1, 0));
        num_paths[1][1] = 1;
        for (int row = 1; row <= m; ++row) {
            for (int col = 1; col <= n; ++col) {
                if (obstacleGrid[row - 1][col - 1] == 1) {
                    continue;
                }
                num_paths[row][col] += num_paths[row - 1][col] + num_paths[row][col - 1];
            }
        }
        return num_paths[m][n];
    }
};
```

#### Code3
```cpp
#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1) {
            return 0;
        }
        vector<vector<int>> num_paths(m + 1, vector<int>(n + 1, 0));
        num_paths[1][1] = 1;
        for (int row = 1; row <= m; ++row) {
            for (int col = 1; col <= n; ++col) {
                if (obstacleGrid[row - 1][col - 1] == 1) {
                    continue;
                }
                num_paths[row][col] += num_paths[row - 1][col] + num_paths[row][col - 1];
            }
        }
        return num_paths[m][n];
    }
};
```
# Step 2
過去のレビューを見て修正([Code4](#Code4))：
- 単なる数字（例えば`1`や`0`）が意味を持つ場合は，定数で置いて意味をわかりやすくする．
  - [Style Guide](https://ttsuki.github.io/styleguide/cppguide.ja.html#:~:text=てください%E3%80%82-,定数,-名)に従い`kObstacle`としたが，この変数名はわかりやすいのか？大文字で`OBSTACLE`のようにした方がわかりやすい気もする．ご意見・採用しているプラクティスなどあればお待ちしております．
- テストケースで`[[]]`や`[]`を考え忘れていた．

#### Code4
```cpp
#include <vector>

#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty()) {
            return 0;
        }

        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        if (obstacleGrid[0][0] == kObstacle || obstacleGrid[m - 1][n - 1] == kObstacle) {
            return 0;
        }

        vector<vector<int>> num_paths(m + 1, vector<int>(n + 1, 0));
        num_paths[1][1] = 1;
        for (int row = 1; row <= m; ++row) {
            for (int col = 1; col <= n; ++col) {
                if (obstacleGrid[row - 1][col - 1] == kObstacle) {
                    continue;
                }
                num_paths[row][col] += num_paths[row - 1][col] + num_paths[row][col - 1];
            }
        }
        return num_paths[m][n];
    }

private:
    static constexpr int kObstacle = 1;
};
```

---
62 Unique Pathsで，DPをするにあたって実は2次元配列はいらないことを思い出す．1次元配列で更新するように作ったのが[Code5](#Coed5)．Step 1で`+=`になっていた更新則部分が`=`に変更されることに注意．
#### Code5
```cpp
#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1) {
            return 0;
        }
        vector<int> num_paths(n, 0);
        num_paths[0] = 1;
        for (int row = 0; row < m; ++row) {
            for (int col = 0; col < n; ++col) {
                if (obstacleGrid[row][col] == 1) {
                    num_paths[col] = 0;
                    continue;
                }
                if (col == 0) {
                    continue;
                }
                num_paths[col] = num_paths[col] + num_paths[col - 1];
            }
        }
        return num_paths[n - 1];
    }
};
```

---
再帰verも書いてみる．[Code6](#Code6)．

#### Code6
```cpp
#include <vector>
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        vector<vector<int>> num_paths(m, vector<int>(n, -1));
        return uniquePathsHelper(m, n, obstacleGrid, num_paths);
    }

private:
    int uniquePathsHelper(const int m, const int n, const vector<vector<int>>& obstacleGrid, vector<vector<int>>& num_paths) {
        if (m == 1 && n == 1) {
            if (obstacleGrid[0][0] == 1) {
                return 0;
            }
            num_paths[0][0] = 1;
            return 1;
        }
        // m,n : [1, 100]
        int row = m - 1;
        int col = n - 1;
        if (obstacleGrid[row][col] == 1) {
            num_paths[row][col] = 0;
            return 0;
        }
        if (row == 0) {
            return uniquePathsHelper(1, n - 1, obstacleGrid, num_paths);;
        }
        if (col == 0) {
            return uniquePathsHelper(m - 1, 1, obstacleGrid, num_paths);;
        }
        if (num_paths[row - 1][col] == -1) {
            num_paths[row - 1][col] = uniquePathsHelper(m - 1, n, obstacleGrid, num_paths);
        }
        if (num_paths[row][col - 1] == -1) {
            num_paths[row][col - 1] = uniquePathsHelper(m, n - 1, obstacleGrid, num_paths);
        }
        return num_paths[row - 1][col] + num_paths[row][col - 1];
    }
};
```

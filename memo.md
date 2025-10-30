*以下の文章内ではLeetCodeの計算時間を参考にしている箇所がありますが，実際にはLeetcodeの計測時間は不正確とのことですので，読み飛ばしていただけたらと思います（後にベンチマークでの計測値に置き換える予定です）*

# Step 1
- 無向グラフの連結成分分解だからDFSかBFSやれば良い．
  - 探索中の座標を突っ込むコンテナをキューにすればBFS，スタックにすればDFSになる．
  - 研究でDFSを使いそうな雰囲気なので，練習がてらDFSでやってみる．
- とりあえず実装してみたがテストランでWrong Answer. テストケースの出力を見ると最終出力に対応する`num_components`がマスの数（4*5のgridなら`num_components == 20`）になっている．
  - `grid`の各成分は`string`型だったので，`grid[i][j] == 1`としてはうまく動かないことに気づき，修正したらAccept（[Code1](#Code1)）．
    - char型はシングルクオーテーションを用いることに注意（ダブルクオーテーションだと文字列リテラル(const char*)型になる．）
- 最終的に通ったのが以下のコード．
  - しかし所要時間が134msで，正答者平均が26msくらいであることと比べると明らかに遅すぎる．
  - `SearchAdjacentPoints()`の`adjacent_diff`を`set`から`vector`にしたら82msくらいに．変えてみた理由としては，
      - 全部舐めるだけならsetよりvectorの方が早そう（所属判定とか要素の変更とかをしないなら，という意味）
        - setは木だから構築にも時間かかりそう
        - 舐める速さ自体は，結局木も配列の形で保持している変わらないかな．
  - stack(DFS)じゃなくてqueue(BFS)にしても速さは変わらないはずだよなあ.
 
### Code1
```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        stack<pair<int, int>> points_searching; // DFS or BFS用の集合 (stack or queue, respectively)
        set<pair<int, int>> points_unvisited; // 未探索点の集合
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '1') points_unvisited.insert({i,j});
            }
        }

        int num_components = 0; // islandの数
        while (!points_unvisited.empty()) {
            ++num_components;
            // 未探索点から1つ点p_をとり，その点から辿れる点をDFSで探索
            auto p_ = points_unvisited.begin();
            points_searching.push(*p_); 
            points_unvisited.erase(p_);

            while (!points_searching.empty()) {
                pair<int, int> p = points_searching.begin();
                points_searching.pop();
                SearchAdjacentPoints(grid, p, points_unvisited, points_searching);
            }
        }

        return num_components;
    }

private:
    void SearchAdjacentPoints(vector<vector<char>>& grid, pair<int, int> point, set<pair<int, int>>& points_unvisited, stack<pair<int, int>>& points_searching) {
        set<pair<int, int>> adjacent_diff = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int i = point.first;
        int j = point.second;
        for (pair<int, int> delta : adjacent_diff) {
            int di = delta.first;
            int dj = delta.second;
            if (IsValidLand(i + di, j + dj, grid) && points_unvisited.contains({i + di, j + dj})) {
                points_searching.push({i + di, j + dj});
                points_unvisited.erase({i + di, j + dj});
            }
        }
    }

    bool IsValidLand(int i, int j, vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        return 0<= i && i < m && 0 <= j && j < n && grid[i][j] == '1';
    }
};
```

# Step 2
* Step 1からずいぶん時間が経ってしまったので，改めて0からコードを書いてみる（[Code2](#Code2)）
* 外から見たメンテナンス性．
  * 例えば，[Code2](#Code2)のprivateな関数たちは予想外の入力に対応できるか？
  * 言い換えると，単に呼び出すだけで使えるか？必要な判定は関数の内部でできているか？を意識する．
* 今回のようなケースで再帰にするとオーバーフローが怖いのでループ化しているが，再帰のメリットってなんだろう？読みやすさ？
  * 例えばC++11以降では再帰の深さは1024回を上限とするのが標準のようだが，`grid`の縦横の上限が300なので最大で90000の再起呼び出しをする可能性がある．したがって今回は再帰は不適切だろう．
* Union findについて調べておく（時間がないので後回しにする）

## その他，調べたこと
* 今回の私のコードには登場しなかったが，[swapでコードを簡潔に書く](https://discord.com/channels/1084280443945353267/1201211204547383386/1213153863704776774)のはいいアイデアだと思った．
* [自然言語での表現通りにコードを書く](https://discord.com/channels/1084280443945353267/1201211204547383386/1213387878734766080)．
* テンプレートについてよくわかっていなかったので確認．
  * テンプレート
    * 目的：「型を後から決めたい」
    * 実現方法：「コンパイル時にコンパイラがコードの中で実際に使われる型を推論して処理」
    * 応用例：クラステンプレート，関数テンプレート，メタプログラミング，コンセプト，etc．
  * 関数テンプレート
    * 目的：「型に依存しないで同じロジックを使いたい」
    * 実現方法：「コンパイラが関数呼び出し時に自動で型を推論し，その型に応じた関数を生成」
* autoは参照を落とす．

### Code2
```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int len_row = grid.size();
        int len_column = grid[0].size();
        set<pair<int, int>> unvisited_points;
        for (int i = 0; i < len_row; ++i) {
            for (int j = 0; j < len_column; ++j) {
                if (grid[i][j] == '1') unvisited_points.insert({i,j});
            }
        }

        int num_islands = 0;
        stack<pair<int, int>> found_points;
        while (!unvisited_points.empty()) {
            ++num_islands;
            auto start_point = unvisited_points.begin();
            found_points.push(*start_point);
            unvisited_points.erase(start_point);
            TraverseAnIsland(grid, found_points, unvisited_points);
        }

        return num_islands;
    }
private:
    void TraverseAnIsland(const vector<vector<char>>& grid,
                          stack<pair<int, int>>& found_points,
                          set<pair<int, int>>& unvisited_points) {
        while (!found_points.empty()) {
            pair<int, int> next_point = found_points.top();
            found_points.pop();
            TraverseAdjacentLand(grid, found_points, unvisited_points, next_point);
        }
    }
    
    void TraverseAdjacentLand(const vector<vector<char>>& grid,
                              stack<pair<int, int>>& found_points,
                              set<pair<int, int>>& unvisited_points,
                              pair<int, int> point) {
        vector<pair<int, int>> adjacent = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        int i = point.first;
        int j = point.second;
        for (auto [di, dj] : adjacent) {
            if (IsValidUnvisitedLand(grid, unvisited_points, {i + di, j + dj})) {
                found_points.push({i + di, j + dj});
                unvisited_points.erase({i + di, j + dj});
            }
        }
    }

    bool IsValidUnvisitedLand(const vector<vector<char>>& grid,
                              set<pair<int, int>>& unvisited_points,
                              pair<int, int> point) {
        int len_row = grid.size();
        int len_column = grid[0].size();
        int i = point.first;
        int j = point.second;
        if (0 <= i && i < len_row && 0 <= j && j < len_column && grid[i][j] == '1') {
            if (unvisited_points.contains({i, j})) return true;
        }
        return false;
    }
};
```

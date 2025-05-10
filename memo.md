# Step 1
- 無向グラフの連結成分分解だからDFSかBFSやれば良い．
  - 探索中の座標を突っ込むコンテナをキューにすればBFS，スタックにすればDFSになる．
  - 研究でDFSを使いそうな雰囲気なので，練習がてらDFSでやってみる．
- プロトタイプはこんな感じだが，コンパイルエラー．`std::set`には`top()`なんてない，と怒られてしまったので`begin()`を用いて要素を取ってくるよう変更した
- しかしテストランでWrong Answer. テストケースの出力を見ると出力の`num_components`がマスの数（4*5のgridなら`num_components == 20`）になっている．
  - `grid`の各成分は`string`型だったので，`grid[i][j]`を用いた条件文が期待通り動いていないことに気づく．
    - char型はシングルクオーテーションを用いることに注意（ダブルクオーテーションだと文字列リテラル(const char*)型になる．）
- 最終的に通ったのが以下のコード．
  - しかし所要時間が134msで，正答者平均が26msくらいであることと比べると明らかに遅すぎる．LeetCodeのRuntimeは正確でないとはいえ改善の余地がありそう．
  - `SearchAdjacentPoints()`の`adjacent_diff`を`set`から`vector`にしたら82msくらいに．変えてみた理由としては，
      - 全部舐めるだけならsetよりvectorの方が早そう（所属判定とか要素の変更とかをしないなら，という意味）
        - setは木だから構築にも時間かかりそう
        - 舐めるの自体は，結局木は配列の形で保持するから変わらないかな．
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
                pair<int, int> p = points_searching.top();
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

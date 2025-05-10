# Step 1
- どう見たって200. Number of Islandsの亜種なので，その時のコードに面積カウント機能をつければ良い
- どう見たって200. Number of Islandsの亜種なのに，今回は`grid`の各成分が`char`じゃなくて`int`になってるって罠でしかない．
- しかしそれを修正してもWrong Answer（以下のコード）．
  - Outputが正答6に対し11とか．
  - DFS自体は間違っていないはずなのでコードを読み直してみる．
  - `maxAreaOfIsland()`本体（`points_searching`から点が削除されるとき）でも`SearchAdjacentPoints()`（`points_searching`に点が追加されるとき）でも`++area_;`をしているが，これだと2重に増やしている．
    - `points_searching`から削除されるときに`++area_;`の方がわかりやすいと思ったのでそっちを残して他方を削除したらAccepted.
```cpp
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        stack<pair<int, int>> points_searching; // DFS or BFS用の集合 (stack or queue, respectively)
        set<pair<int, int>> points_unvisited; // 未探索点の集合
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) points_unvisited.insert({i,j});
            }
        }

        int num_components = 0; // islandの数
        int max_area = 0; // areaの最大値
        while (!points_unvisited.empty()) {
            ++num_components;
            area_ = 0;
            // 未探索点から1つ点p_をとり，その点から辿れる点をDFSで探索
            auto p_ = points_unvisited.begin();
            points_searching.push(*p_); 
            points_unvisited.erase(p_);

            while (!points_searching.empty()) {
                ++area_;
                pair<int, int> p = points_searching.top();
                points_searching.pop();
                SearchAdjacentPoints(grid, p, points_unvisited, points_searching);
            }
            max_area = max(area_, max_area);
        }

        return max_area;
    }

private:
    int area_;
    void SearchAdjacentPoints(vector<vector<int>>& grid, pair<int, int> point, set<pair<int, int>>& points_unvisited, stack<pair<int, int>>& points_searching) {
        set<pair<int, int>> adjacent_diff = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int i = point.first;
        int j = point.second;
        for (pair<int, int> delta : adjacent_diff) {
            int di = delta.first;
            int dj = delta.second;
            if (IsValidLand(i + di, j + dj, grid) && points_unvisited.contains({i + di, j + dj})) {
                points_searching.push({i + di, j + dj});
                points_unvisited.erase({i + di, j + dj});
                ++area_;
            }
        }
    }

    bool IsValidLand(int i, int j, vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        return 0<= i && i < m && 0 <= j && j < n && grid[i][j];
    }
};
```


# Step 2

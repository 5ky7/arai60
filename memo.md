# Step 1
* DFSかWFSでTraverseすれば島が一つ見つかる
  * これの前に解いた問題ではDFSを再帰で書いたので，ここではWFSをループで書いた（[Code1](#Code1))
    * 問題文の制約`0 <= ai <= bi < n`が満たされておらずWAくらったので，それに合わせて書きなおしたがTLE．
* 上のコードを書いていて，辺に関するデータの形がしっくり来なかったのでそこを描き直してみる
 * グラフの辺の持ち方は行列形式か，連結リスト形式がある，ということを習った記憶
  * 行列だと各成分定数アクセスできるので密なグラフに強い一方，O(頂点の数^2)のメモリ消費
  * リスト形式だとメモリ消費がO(辺)に抑えられるので疎なグラフに対して効率的だが，ある頂点から出る辺を調べる時O(出次数)の計算量
 * 今回は,`頂点の数^2 < 2000^2`なのでvector<vector<int>>で保持したら`1.6 * 10^7 B = 16MB`．まあプログラムが回らないこともないだろう．
 * 一方で`辺の数 < 5000`なので`頂点の数^2`の最大値よりかは大分小さい．まあ行列，リストどっちでも良いだろう．
 * 行列で書いたのが[Code2](#Code2), 連結リストで書いたのが[Code3](#Code3).
 
### Code1
```cpp
class Solution {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        if (n == 0) {
            return 0;
        }

        set<vector<int>> set_edges(edges.begin(), edges.end());
        vector<int> visited(n, 0); // 0: unvisited, 1: visited
        deque<int> searching_points;

        int num_components = 0;
        for (int i = 0; i < n; ++i) {
            if (visited[i] == 1) {
                continue;
            }

            ++num_components;
            searching_points.push_back(i);
            while (!searching_points.empty()) {
                int current_point = searching_points.front();
                searching_points.pop_front();
                visited[current_point] = 1;

                for (int neighbor_point = i + 1; neighbor_point < n; ++neighbor_point) {
                    if (!(set_edges.contains({current_point, neighbor_point}) ||
                        set_edges.contains({neighbor_point, current_point}))) {
                        continue;
                    }
                    if (visited[neighbor_point] == 0) {
                        searching_points.push_back(neighbor_point);
                    }
                    continue;
                }
            }
        }

        return num_components;
    }
};
```
### Code2
```cpp
class Solution {
private:
    int n_;
    void TraverseOneComponent(const vector<vector<int>>& adjacent_matrix, int i, vector<int>& visited) {
        visited[i] = 1;

        for (int j = 0; j < n_; ++j) {
            if (adjacent_matrix[i][j] == 1 && visited[j] == 0) {
                TraverseOneComponent(adjacent_matrix, j, visited);
            }
        }
    }
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        if (n == 1) {
            return 1;
        }

        n_ = n;
        vector<int> visited(n, 0);
        vector<vector<int>> adjacent_matrix(n, vector<int>(n, 0));
        for (vector<int> edge : edges) {
            int i = edge[0];
            int j = edge[1];
            adjacent_matrix[i][j] = 1;
            adjacent_matrix[j][i] = 1;
        }

        int num_components = 0;
        for (int i = 0; i < n_; ++i) {
            if (visited[i] == 1) {
                continue;
            }
            TraverseOneComponent(adjacent_matrix, i, visited);
            ++num_components;
        }

        return num_components;
    }
};
```
### Code3
```cpp
class Solution {
private:
    int n_;
    void TraverseOneComponent(const vector<vector<int>>& connection, int i, vector<int>& visited) {
        visited[i] = 1;

        for (int neighbor : connection[i]) {
            if (visited[neighbor] == 0) {
                TraverseOneComponent(connection, neighbor, visited);
            }
        }
    }
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        if (n == 1) {
            return 1;
        }

        n_ = n;
        vector<int> visited(n, 0);
        vector<vector<int>> connection(n);
        for (vector<int> edge : edges) {
            int i = edge[0];
            int j = edge[1];
            connection[i].push_back(j);
            connection[j].push_back(i);
        }

        int num_components = 0;
        for (int i = 0; i < n_; ++i) {
            if (visited[i] == 1) {
                continue;
            }
            TraverseOneComponent(connection, i, visited);
            ++num_components;
        }

        return num_components;
    }
};
```
# Step 2

# Step 1
* DFSかWFSでTraverseすれば島が一つ見つかる
  * これの前に解いた問題ではDFSを再帰で書いたので，ここではWFSをループで書いた（[Code1](#Code1))
    * 問題文の制約`0 <= ai <= bi < n`が満たされておらずWAくらったので，それに合わせて書きなおしたがTLE．
 
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
# Step 2

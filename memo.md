# Step 1
- 先頭から見ていけば良い．（1つ目のコード，Wrong Answer）
  - `(nums1[0], nums2[0])`は最小の和を作るペア
  - その次に小さいのは`(nums1[0], nums2[1])`か`(nums1[1], nums2[0])`であるから，小さい方を採用していけば良い
  - よって`index1`と`index2`を用意して，`nums1[index1 + 1] + nums2[index2]`と`nums1[index1] + nums2[index2 + 1]`を比較しては`index1,index2`を進めていけば良い
- と思ったが，これだと`index1,index2`が今より小さいケースを考えられていない（考え漏らしているものがある）．
- 漏れが嫌なら，総当たりでソートすれば良いのでは？-->`priority_queue`の利用．（2つ目のコード，Memory Limit Exceeded）
  - 総当たりだと`nums1, nums2`の長さがクソデカの時にメモリの使用量が爆発する（配列サイズの積＝n^2のオーダー）
- 回答を見ると，i<jなら全てのkについて`nums1[k] + nums2[i] < nums1[k] + nums2[j]`を利用してメモリ使用量をnのオーダーに抑えたものが．（3つ目のコード）
  - まず全ての`i`について`(nums1[i], nums2[0])`のペアを格納して，pq.top()で`nums1[j]`を含むペアが選ばれてpopされたら`(nums1[j], nums2[1])`を入れていけば良い．
  - indexが配列の長さを超えないか判定するのを忘れていたので，追加．
  - priority_queueを昇順にするのは宣言時に比較関数を渡した方がメンテしやすそうなので変更．
- データ保持の時に，データそのものとindex（orデータの場所），どちらを保持するのか
  - 今回はindexに意味（`i<j` なら `nums[i] < nums[j]`）があるから，indexを保持.
  - というかデータそのものを保持したいいケースってあるのか？アクセスに時間がかかる時とか？
```c++:from_top.cpp
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        std::vector<std::vector<int>> pairs_with_k_smallest_sums;
        int index1 = 0; // nums1用のindex
        int index2 = 0; // nums2用のindex
        int len_nums1 = nums1.size();
        int len_nums2 = nums2.size();

        pairs_with_k_smallest_sums.push_back({nums1[0], nums2[0]}); // 確定で最小なので追加

        int next1, next2; // index1のみを一つ進めた和とindex2のみを1つ進めた和
        for (int i = 1; i < k; i++) {
            if ((index1 < len_nums1 - 1) && (index2 < len_nums2 - 1)) {
                next1 = nums1[index1 + 1] + nums2[index2];
                next2 = nums1[index1] + nums2[index2 + 1];
                if (next1 <= next2) {
                    ++index1;
                } else {
                    ++index2;
                }
                pairs_with_k_smallest_sums.push_back({nums1[index1], nums2[index2]});
                continue;
            }
            if (index2 == len_nums2 - 1) {
                ++index1;
                pairs_with_k_smallest_sums.push_back({nums1[index1], nums2[index2]});
                continue;
            }
            if (index1 == len_nums1 - 1) {
                ++index2;
                pairs_with_k_smallest_sums.push_back({nums1[index1], nums2[index2]});
                continue;
            }
        }

        return pairs_with_k_smallest_sums; 
    }
};
```
```c++:priority_queue.cpp
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        int len_nums1 = nums1.size();
        int len_nums2 = nums2.size();
        std::priority_queue<pair<int, vector<int>>> pq_sum_and_pair;
        
        for (int i = 0; i < len_nums1; ++i) {
            for (int j = 0; j < len_nums2; ++j) {
                pq_sum_and_pair.push(
                    {
                        - nums1[i] - nums2[j], // デフォルトは降順ソートなので負で格納．
                        {nums1[i], nums2[j]} 
                    }
                );
            }
        }

        std::vector<vector<int>> solution;
        for (int l = 0; l < k; ++l) {
            solution.push_back(pq_sum_and_pair.top().second);
            pq_sum_and_pair.pop();
        }

        return solution;
    }
};
```
```c++
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        int len_nums1 = nums1.size();
        int len_nums2 = nums2.size();
        std::priority_queue<
            pair<int, int>,
            std::vector<pair<int, int>>,
            std::greater<pair<int, int>>
        > pq_sum_and_index2;
        
        for (int i = 0; i < len_nums1; ++i) {
            pq_sum_and_index2.push({nums1[i] + nums2[0], 0});
        }

        int i2; // nums2用のindex
        int sum;
        std::vector<vector<int>> solution;
        for (int l = 0; l < k; ++l) {
            sum = pq_sum_and_index2.top().first;
            i2 = pq_sum_and_index2.top().second;
            solution.push_back({sum - nums2[i2], nums2[i2]}); // (nums1[i1], nums2[i2])を追加．
            pq_sum_and_index2.pop();
            if (i2 < len_nums2 - 1) {
                pq_sum_and_index2.push({sum - nums2[i2] + nums2[i2 + 1], i2 + 1});
            }
        }

        return solution;
    }
};
```



# Step 2
- heapの構造を実装してみた（諸事情によりPython）
```python
class heap_:
  def __init__(self, nums):
    self.tree = []
    if nums:
      for num in nums:
        self.add(num)
    
  def add(self, x):
    self.tree.append(x)
    len_tree = len(self.tree)
    i_x = len_tree - 1
    i_parent = (i_x - 1)// 2
    while i_x > 0 and x > self.tree[i_parent]:
      self.tree[i_parent], self.tree[i_x] = self.tree[i_x], self.tree[i_parent]
      i_x = i_parent
      i_parent = (i_x - 1) // 2

  
  def max(self):
    if not self.tree:
      return None
    return self.tree[0]
  
  
  def pop(self):
    if not self.tree:
      return None
    if len(self.tree) == 1:
      self.tree = []
      return None
    
    self.tree[0] = self.tree.pop()
    self._heapify_down(0)
 
  def _heapify_down(self, i):
    while True:
      left = 2*i + 1
      right = 2*i + 2
      largest = i

      len_tree = len(self.tree)
      if left < len_tree and self.tree[largest] < self.tree[left]:
        largest = left
      elif right < len_tree and self.tree[largest] < self.tree[right]:
        largest = right
      
      if largest != i:
        self.tree[i], self.tree[largest] = self.tree[largest], self.tree[i]
      else:
        break
```

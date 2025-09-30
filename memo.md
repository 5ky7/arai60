問題:[373.-Find-K-Pairs-with-Smallest-Sums](https://leetcode.com/problems/find-k-pairs-with-smallest-sums)
# Step 1
- 先頭から見ていけば良いと考えた（がポインタの進め方が間違っていた）．（1つ目のコード，Wrong Answer）
  - `(nums1[0], nums2[0])`は最小の和を作るペア
  - その次に小さいのは`(nums1[0], nums2[1])`か`(nums1[1], nums2[0])`であるから，小さい方を採用していけば良い
  - よって`index1`と`index2`を用意して，`nums1[index1 + 1] + nums2[index2]`と`nums1[index1] + nums2[index2 + 1]`を比較しては`index1,index2`を進めていけば良い
- これだと`index1,index2`が今より小さいケースを考えられていない（考え漏らしているものがある）．
  - 例えば`nums1 =[1,2,4,5,6], nums2 =　[3,5,7,9]`とかだと，最小ペアは`[1,3], [2,3], [1,5]`になるはずだが，この考え方だと`[2,3]`を見た後に`[4,3]`と`[2,5]`しか見れていない．
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
* ヒープの中身が不自然だったので，自分で比較関数を作ってindexのペアを保持するように変更
  * ヒープに要素を追加する際，indexのチェックを忘れて一度Wrong Answer(`sorted_index_pairs.push(std::make_pair(p.first, p.second + 1));`のところ）
```cpp
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        // indexのペアを要素にもつpriority_queue用の比較関数
        // nums1[i] + nums2[j]が小さい(i,j)が先頭に来る．
        auto comp = [&nums1, &nums2](pair<int,int> i_pair, pair<int, int> j_pair) {
            return nums1[i_pair.first] + nums2[i_pair.second] > nums1[j_pair.first] + nums2[j_pair.second];
        };

        std::priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> sorted_index_pairs(comp);
        for (int i = 0; i < nums1.size(); ++i) {
            sorted_index_pairs.push(std::make_pair(i, 0));
        }

        vector<vector<int>> k_num_pairs_smallest_sum(k);
        for (int i = 0; i < k; ++i) {
            auto p = sorted_index_pairs.top();
            k_num_pairs_smallest_sum[i] = vector<int>{nums1[p.first], nums2[p.second]};
            sorted_index_pairs.pop();
            if (p.second < nums2.size() - 1) {
                sorted_index_pairs.push(std::make_pair(p.first, p.second + 1));
            }
        }

        return k_num_pairs_smallest_sum;
    }
};
```

* 構造体を用意してみる
* priority queueに`nums1[i] + nums2[0]`を全てのiについて入れるのではなく，`nums1[0] + nums2[0]`を入れて必要に応じて`nums1[i] + nums2[0]`を追加していく
  * こうすることで**見る必要のない要素*を見る必要がなくなる
    * 例えばnums1の値がどれも大きく，nums2の値がどれも小さい場合は，実質的に`nums1[(大きい値)] + nums2[(小さい値)]`を見ずに`nums1[(小さい値)] + nums2[(大きい値)]`のみを見ていけば良い
```cpp
class Solution {
private:
    struct IndexesAndSum {
        int i1;
        int i2;
        int sum;
    };

    struct Greater {
        bool operator()(IndexesAndSum a, IndexesAndSum b) const {
            return a.sum > b.sum;
        }
    };
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        std::priority_queue<IndexesAndSum, vector<IndexesAndSum>, Greater> indexes_and_sum;
        indexes_and_sum.push({0, 0, nums1[0] + nums2[0]});

        std::vector<std::vector<int>> k_smallest_pairs(k, std::vector<int>(2));
        for (int i = 0; i < k; ++i) {
            auto [i1, i2, sum] = indexes_and_sum.top();
            indexes_and_sum.pop();
            k_smallest_pairs[i] = {nums1[i1], nums2[i2]};

            if (i1 + 1 < nums1.size()) {
                indexes_and_sum.push({i1 + 1, i2, nums1[i1 + 1] + nums2[i2]});
            }
            if (i1 == 0 && i2 + 1 < nums2.size()) {
                indexes_and_sum.push({i1, i2 + 1, nums1[i1] + nums2[i2 + 1]});
            }
        }

        return k_smallest_pairs;
    }
};
```

# Step 1
- nums内の要素とその登場回数のペアを扱いたい．特に登場回数が変化し，かつ登場回数の大小を扱いたい（ソート，あるいは最大の取得）
  - `vector<int>`で引数をスコアに，値を登場回数に対応させる
      - 空間計算量がとんでもないことになりそうだし，でかい配列になるからソートも時間かかりそうなので却下．
  - `std::map<int, int>`は，`map['hoge']++`で登場回数を変化させられるが，ソートが登場回数ではなく要素（キー）によってなされる
    - プライオリティキューと同様，キーが最小（or最大）の要素をO(1)で取得できる勘違いしていたが，赤黒木で実装されているためそんなことはない．キー最大or最初を辿るためO(log N)必要．
  - `std::priority_que<pair<int, int>>`
    - 最大をO(1)で取得可能，O(log N)で取得&削除可能なので良さげ．ただし一度入れた要素を変更する（登場回数を数えていく）のはめんどくさい．
  - `std::vector<pair<int, int>>` 
    - 新しい要素が来るたびにソートし直せば各ソートがO(Nlog N)でできるからlog Nを定数と見て仕舞えばO(N^2)くらい
- ふと思った点として，「*登場回数が変化*」と述べたとおり動的に保持する前提だったが，703. Kth Largest Element in a Streamと違って新しく追加される機能はないので，静的で良い？
  - 一旦`std::map`に入れていって(O(log N))，それを順次`std::priority_queue`に入れていけば(O(log N))，ロジックがシンプルかつトータル計算量O(log N)でいい感じ．これを実装したらAccepted（以下のコード）

```c++
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std:map<int, int> map_num_appear; // mapの要素は{スコア, 登場回数}
        for (int i : nums) {
            map_num_appear[i]++;
        }

        std::priority_queue<std::pair<int, int>> pq_num_appear; // 各要素は登場回数でソートされるように{登場回数, スコア}にする
        for (auto pair : map_num_appear) {
            std::pair<int, int> pair_transpose = {pair.second, pair.first};
            pq_num_appear.push(pair_transpose);
        }

        vector<int> k_most_frequent_elements; // 出力用
        for (int i = 0; i < k; i++) {
            k_most_frequent_elements.push_back(pq_num_appear.top().second);
            pq_num_appear.pop();
        }

        return k_most_frequent_elements;
    }
};
```
 

# Step 2

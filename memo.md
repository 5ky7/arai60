# Step 1
- 単純な総当たりでやると時間計算量O(N^2)，空間計算量O(1)（以下のコード）
  - TLEになると思ったが，普通に通った．サイズは最大10^4なのできついかと思った．各ループ内の処理が単純なので定数倍が小さかったか？
  - テストケースが少ないのでなんか色々怪しいかもしれない．
  - イテレータで配列を舐めるのと，indexで配列を舐めるの，どっちが早いか気になったが最適化ビルドでは変わらないらしい．
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int len_nums = nums.size();
        for (int i = 0; i < len_nums; ++i) {
            for (int j = i+1; j < len_nums; ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }

        throw invalid_argument("There is no solution");
    }
};
```
- というわけでHash mapを用いた高速化を試みる．
  - C++では`unordered_map`がhash mapを内部データ構造に用いているのでこれを利用．
  - 平均時間計算量，空間計算量ともにO(N)，最悪時間計算量は，ハッシュマップが全部衝突してかつ`nums`の最後の要素まで調べるときで，forループがN回，`contains()`でO(N)，`insert()`でO(N)の計O(N^3).
    - いやまあ，ハッシュが全衝突とかありえないと思うが...
  - `map`は赤黒木．要素の検索および追加にO(log N)なのでこっちを使うと時間計算量がO(NlogN)になる．
  - が，結局定数倍次第だし，log Nはほぼ定数なのでどちらでも良さそう．
  - LeetCodeだとunorderd_map, mapでそれぞれ3msで14.8MB, 4msで15.0MBなのでやや前者が良い
    - とはいえLeetCodeの時間は結構不正確らしいので，やはりなんともいえない．
```
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> map_nums;

        for (int i = 0; i < nums.size(); ++i) {
            // nums[i]と足してtargetになるようなnums[j]があればjを返して終わり
            if (map_nums.contains(target - nums[i])) {
                return {i, map_nums[target - nums[i]]};
            }
            // そんなjがなければiをデータセットに追加
            map_nums.insert({nums[i],i});
        }
        throw invalid_argument("There is no solution");
    }
};
```
- これ`unordered_set`でよくね？と思ったがindexを保持しなきゃなんだった．mapじゃないとダメ．

# Step 2
- O(Nlog N)でソートしてから何かやるのもありかもしれない．
  - と思ったが，indexを返すんだった．現実的ではなさそう．


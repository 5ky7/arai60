# Step 1
- いつだか解いたものが[Code1](#Code1)．
- 時間が経ったので解き直してみる．所要時間22分．
  - とりあえず総当たりを書いてみる．[Code2](#Code2)．5分程度
    - 時間計算量O(n^2), 空間計算量O(1)．
    - `nums`に負の数が含まれていることを忘れて`max_sum_subarray`の初期値を0にしてしまった．`numeric_limits<int>::min()`に修正するも，（当然）TLE．
  - 工夫を考える．
    - 一つ小さいサイズの問題の解を参照するのは不可能ぽい．`[2, -1, 3]`に対して`[2,1]`の解は`[2]`だから，ここから`[2,-1,3]`を構築するのは無理そう．
    - 連続部分列だから，ある`nums[i]`を部分列に含むことができるのは`nums[i-1]`が部分列に含まれる時のみ．
    - ということは，一つ小さいサイズを参照するには，問題を変えて，「`nums[i]`を最後に含む部分列で最大のもの」を，「`nums[i-1]`を最後に含む部分列で和が最大のものと，`nums[i-1]`を含まないもの（＝空配列）」を比べれば良い．
    - この発想で書いたのが[Code3](#Code3)．
      - 時間計算量O(n), 空間計算量O(n)．

### Code1
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        vector<int> max_length_with_itself(nums.size());
        max_length_with_itself[0] = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            max_length_with_itself[i] = nums[i];
            if (max_length_with_itself[i - 1] > 0) {
                max_length_with_itself[i] += max_length_with_itself[i - 1];
            }
        }

        return *max_element(max_length_with_itself.begin(), max_length_with_itself.end());
    }
};
```

### Code2
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int sum_subarray = 0;
        int max_sum_subarray = 0;
        for (int first = 0; first < nums.size(); ++first) {
            sum_subarray += nums[first];
            max_sum_subarray = std::max(max_sum_subarray, sum_subarray);
            for (int last = first + 1; last < nums.size(); ++last) {
                sum_subarray += nums[last];
                max_sum_subarray = std::max(max_sum_subarray, sum_subarray);
            }
            sum_subarray = 0;
        }
        return max_sum_subarray;
    }
};
```

### Code3
```cpp
#include <vector>
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        std::vector<int> max_subarray_with_itself(nums.size());
        max_subarray_with_itself[0] = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            max_subarray_with_itself[i] = nums[i];
            if (max_subarray_with_itself[i - 1] > 0) {
                max_subarray_with_itself[i] += max_subarray_with_itself[i - 1];
            }
        }

        return *max_element(max_subarray_with_itself.begin(), max_subarray_with_itself.end());
    }
};
```
# Step 2

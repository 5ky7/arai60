[198. House Robber](https://leetcode.com/problems/house-robber/)
# Step 1
過去に解いたものが[Code1](#Code1)．

時間が経ったので解き直す．[Code2](#Code2)．入力制約から見て`int`で扱えば十分．

#### Code1
```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() <= 2) {
            return *max_element(nums.begin(), nums.end());
        }
        vector<int> max_robbery(nums.size());
        max_robbery[0] = nums[0];
        max_robbery[1] = max(nums[1], nums[0]);
        for (int i = 2; i < nums.size(); ++i) {
            max_robbery[i] = max(max_robbery[i - 2] + nums[i], max_robbery[i - 1]);
        }

        return *(max_robbery.end() - 1);
    }
};
```

#### Code2
```cpp
#include <vector>

class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        if (nums.size() <= 2) {
            return *max_element(nums.begin(), nums.end());
        }

        vector<int> max_amount_to(nums.size(), 0);
        max_amount_to[0] = nums[0];
        max_amount_to[1] = std::max(nums[0], nums[1]);
        for (int i = 2; i < nums.size(); ++i) {
            max_amount_to[i] = std::max(max_amount_to[i - 1], max_amount_to[i - 2] + nums[i]);
        }

        return max_amount_to.back();
    }
};
```

---
- テストケースを考える．
    - 要素数1
        - `[1]`
    - 要素数2
        - `[1,2]`
        - `[2,1]`
        - `[0,1]`
        - `[1,0]`
    - 要素数 > 2
        - 答えが1つ飛ばしの部分列
            - `[1,2,3,1]`
            - `[2,1,1,3]`
        - 答えが2つ飛ばしの部分列
            - `[3,1,2,4]`
            - `[1,4,1,1,4]`
        - 答えが3つ飛ばしの部分列 -> 存在しない（3つ飛ばすなら真ん中のやつを追加できる）

# Step 2
[Code3](#Code3)．修正点は以下の通り．
- 変数名の修正．[参考](https://github.com/dxxsxsxkx/leetcode/pull/35/changes#diff-37bde376c719d629667a55f34ac8dfed6b5a2d774d005fcb49bafb0a7410a00fR10)．

---
この問題も，[2つ前までさえ保持していれば良い](https://github.com/dxxsxsxkx/leetcode/pull/35/changes#diff-e037c6bae3811b36d8b1ffc2cb0170188d49c4ce3907166d1be2642e7493f85fR31)からテーブルはいらない．ということで書いたのが[Code4](#Code4)．なお，`prev_prev_max`,`prev_max`の初期値は[DPのコード](#Code2)と異なり[0で良い](https://github.com/dxxsxsxkx/leetcode/pull/35/changes#r2835907619)．こうするとループも`i=1`から回せて綺麗．

なお，[max_amountのスコープはfor文の中のみで良い](https://github.com/dxxsxsxkx/leetcode/pull/35/changes#r2835909356)が，`return prev_max`より`return max_amount`の方がわかりやすいと感じたのでこちらを採用．

まあ普通に配列を使った方が読みやすくはある．空間計算量も$`O(n)`$でボトルネックにはなりにくそうだし．`nums`じゃなくて，何らかのデータの配列が与えられて各要素がめちゃくちゃ重い，とかだとテーブル無しverの出番かも．

#### Code4
```cpp
#include <vector>

class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        if (nums.size() <= 2) {
            return *max_element(nums.begin(), nums.end());
        }

        int prev_prev_max = 0;
        int prev_max = 0;
        int max_amount = 0;
        for (int ith_house = 1; ith_house <= nums.size(); ++ith_house) {
            max_amount = std::max(prev_max, prev_prev_max + nums[ith_house - 1]);
            prev_prev_max = std::max(prev_prev_max, prev_max);
            prev_max = std::max(prev_max, max_amount);
        }

        return max_amount;
    }
};
```

---
再帰verも書いてみる．[Code5](#Code5)．`std::max_element(first, last);`の走査範囲は`[fisrt, last)`なので`last`は含まないことに注意．

#### Code5
```cpp
#include <vector>

class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }

        vector<int> max_amount_to(nums.size(), -1);
        int last_index = nums.size() - 1;
        return MaxAmount(last_index, nums, max_amount_to);
    }

private:
    int MaxAmount(const int i, const vector<int>& nums, vector<int>& max_amount_to) {
        if (i < 0 || i >= nums.size()) {
            throw std::out_of_range("`i` must be in [0, `nums.size()`)");
        }

        if (max_amount_to[i] != -1) {
            return max_amount_to[i];
        }
        if (i <= 1) {
            max_amount_to[i] = *max_element(nums.begin(), nums.begin() + i + 1);
            return max_amount_to[i];
        }
        max_amount_to[i] = std::max(MaxAmount(i - 1, nums, max_amount_to), 
                                    MaxAmount(i - 2, nums, max_amount_to) + nums[i]);
        return max_amount_to[i];
    }
};
```

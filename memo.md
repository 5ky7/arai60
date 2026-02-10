# Step 1
* 過去に解いたものが[Code1](#Code1).
* 時間が経ったので解き直してみたのが[Code2](#Code2).所要時間28分
  * 一旦「連続な部分列」だと勘違いして解いた．つまり入力`[10,9,2,5,3,7,101,18]`に対して`[2, 3, 7, 101]`でなくて`[3, 7, 101]`が出力になるように書いてしまった．
  * 書き直そうとしたが，`[1, 5, 2, 6, 3, 7, 4, 5]`に対して面倒臭そうと思った．つまり部分列を大量に保持して比べる（あるいはO(n^2)で総当たりする）べきか？と思った
  * しかし一つの部分列を持っていれば良いことに気づいた．大量に保持せずとも部分列の一部を**最も効率的（つまり含まれる数がより小さい）な部分列**になるように更新していけば，もしその部分列がもとの部分列より長くなるなら勝手に全体が更新されるし，そうでないなら一部のみ更新されるが長さの情報は保持されたままになることに気づいた．

### Code1
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> longest_subseq;
        for (int num : nums) {
            if (longest_subseq.empty() || num > longest_subseq.back()) {
                longest_subseq.push_back(num);
                continue;
            }
            for (int i = 0; i < longest_subseq.size(); ++i) {
                if (num <= longest_subseq[i]) {
                    longest_subseq[i] = num;
                    break;
                }
            }
        }
        return longest_subseq.size();
    }
};
```

### Code3
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        vector<int> increasing_subsequence(1, nums.front());
        int max_len = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if (increasing_subsequence.back() < nums[i]) {
                increasing_subsequence.push_back(nums[i]);
                continue;
            }
            UpdateSubsequence(nums[i], increasing_subsequence);
        }
        return increasing_subsequence.size();
    }

private:
    void UpdateSubsequence(int new_num, vector<int>& nums) {
        // numsの要素でnew_numより以下の数のうち，最大のものをnew_numで置き換える
        for (int i = 0; i < nums.size(); ++i) {
            if (new_num <= nums[i]) {
                nums[i] = new_num;
                break;
            }
        }
        return;
    }
};
// [1, 5, 2, 6, 3, 7, 4, 5]
// [1, 2, 3, 4, 5]

// [10,9,2,5,3,7,101,18]
// [2, 3, 7, 18]
```

# Step 2

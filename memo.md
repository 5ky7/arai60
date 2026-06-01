[209. Minimum Size Subarray Sum](https://leetcode.com/problems/minimum-size-subarray-sum/)
# Step 1

とりあえず解いてみたのが[Code1](https://leetcode.com/problems/minimum-size-subarray-sum/description/?envType=problem-list-v2&envId=n97t3qfj)
#### Code1
```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int start = 0;
        int end = 0;
        int sum_subarray = 0;
        int min_len_subarray = nums.size() + 1;
        while (end < nums.size()) {
            sum_subarray += nums[end];
            if (sum_subarray < target) {
                ++end;
                continue;
            }
            while (target <= sum_subarray - nums[start]) {
                sum_subarray -= nums[start];
                ++start;
            }
            min_len_subarray = std::min(min_len_subarray, end - start + 1);
            ++end;
        }
        if (min_len_subarray == nums.size() + 1) {
            return 0;
        }
        return min_len_subarray;
    }
};
```

---

冷静に考えると分岐をもっとシンプルにできた．
```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int start = 0;
        int end = 0;
        int sum_subarray = 0;
        int min_len_subarray = nums.size() + 1;
        while (end < nums.size()) {
            sum_subarray += nums[end];
            while (target <= sum_subarray) {
                min_len_subarray = std::min(min_len_subarray, end - start + 1);
                sum_subarray -= nums[start];
                ++start;
            }
            ++end;
        }
        if (min_len_subarray == nums.size() + 1) {
            return 0;
        }
        return min_len_subarray;
    }
};
```

# Step 2

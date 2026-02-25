# Step 1
過去に解いたものが[Code1](#Code1)．

時間が経ったので解き直す．

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
# Step 2

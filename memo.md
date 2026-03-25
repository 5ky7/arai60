# Step 1
[Code1](#Code1)．15分くらい．`first = 1`の初期値からして，`nums.size() == 1`の時はどうしよう，とか色々考えてたらややこしくなり，時間がかかってしまった．
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        int target = nums.front();
        int first = 1;
        int last = nums.size();
        // targetより大きい数たちと，targetより小さい数たちの境界（つまり，小さい数の最小）を探す．
        while (first < last) {
            int mid = first + (last - first) / 2;
            if (nums[mid] > target) {
                first = mid + 1;
            } else {
                last = mid;
            }
        }
        // 存在すればfirstがそこに到達する．
        // 存在しない場合はfirstはnums.end()の位置に到達する．
        return first < nums.size() ? nums[first] : nums[0];
    }
};
```

# Step 2

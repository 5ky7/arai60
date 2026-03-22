[35. Search Insert Position](https://leetcode.com/problems/search-insert-position/)
# Step 1
とりあえず`lower_bound()`使えばいいじゃんというのが[Code1](#Code1)．

#### Code1
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        auto it = std::lower_bound(nums.begin(), nums.end(), target);
        int pos = std::distance(nums.begin(), it);
        return pos;
    }
};
```

---

流石に問題の趣旨はこの`lower_bound`を実装せよ，ということだと思うので書いてみたのが[Code2](#Code2)．
#### Code2
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int first = 0;
        int last = nums.size();
        while (first < last) {
            int mid = first + (last - first) / 2;

            if (nums[mid] == target) {
                return mid;
            }
            if (nums[mid] < target) {
                first = mid + 1;
                continue;
            }
            if (nums[mid] > target) {
                last = mid;
                continue;
            }
        }
        return first;
    }
};
```

---

再帰で書いてみたのが[Code3](#Code3)．
#### Code3
```cpp
class Solution {
public:
    int searchInsert(const std::vector<int>& nums, const int target) {
        return searchInsertHelper(nums, target, 0, nums.size());
    }
private:
    int searchInsertHelper(const std::vector<int>& nums, int target, int first, int last) {
        if (first >= last) {
            return first;
        }

        int mid = first + (last - first) / 2;
        if (nums[mid] == target) {
            return mid;
        }

        if (nums[mid] < target) {
            return searchInsertHelper(nums, target, mid + 1, last);
        } else {
            return searchInsertHelper(nums, target, first, mid);
        }
    }
};
```

---

過去に解いたのが[PastCode](#PastCode)．
#### PastCode
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size();
        int center = (left + right) / 2;
        
        while (right - left > 0) {
            if (nums[center] == target) { return center };
            if (nums[center] > target) { right = center };
            if (nums[center] < target) { left = center + 1 };
            center = (left + right) / 2;
        }

        return center;
    }
};
```

# Step 2

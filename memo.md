[33. Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array/)
# Step 1
とりあえず解いたのが[Code1](#Code1)．

#### Code1
```cpp
class Solution {
public:
    int search(const std::vector<int>& nums, int target) {
        int pos_min = FindMinIndex(nums);
        if (target > nums.back()) {
            auto it = std::lower_bound(nums.begin(), nums.begin() + pos_min, target);
            int pos_insert_target = std::distance(nums.begin(), it);
            return *it == target ? pos_insert_target : -1;
        } else {
            auto it = std::lower_bound(nums.begin() + pos_min, nums.end(), target);
            int pos_insert_target = std::distance(nums.begin(), it);
            if (it == nums.end() || *it != target) {
                return -1;
            }
            return pos_insert_target;
        }
    }
private:
    int FindMinIndex(const std::vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        int target = nums.front();
        int first = 1;
        int last = nums.size();
        while (first < last) {
            int mid = first + (last - first) / 2;
            if (nums[mid] > target) {
                first = mid + 1;
            } else {
                last = mid;
            }
        }
        return first < nums.size() ? first : 0;
    }
};
```

---
自前の関数を用意するよりも標準ライブラリで済まそう（使えるなら全て`lower_bound`を使おう），と思い，[Code2](#Code2)．

意外とスッキリはしない．`it_min`の位置が微妙で，`lower_bound`の未定義動作を事前に回避する必要がある（一応テストは通るみたいだが）．[Code1](#Code1)では`pos_min`を求める`FindMinIndex`が自動で`pos_min == nums.size()`の時に`pos_min = 0`にする処理が入っていたので不要だった．mod `nums.size()`をとってみるのは何故かうまくいかない．ただまあ自前で色々用意せずとも標準ライブラリにあるもので実現できているのでこちらの方が好み．

#### Code2
```cpp
class Solution {
public:
    int search(const vector<int>& nums, int target) {
        std::vector<int> is_less_than_first_num(nums.size());
        std::transform(
            nums.begin(),
            nums.end(),
            is_less_than_first_num.begin(),
            [&nums](int num) {
                return num < nums[0];
            }
        );
        // it_min : nums[0]より小さい要素で一番indexの大きいもの（存在しない場合はnums.end())
        auto it_min = std::upper_bound(is_less_than_first_num.begin(), is_less_than_first_num.end(), 0);
        int pos_min = std::distance(is_less_than_first_num.begin(), it_min);
        if (target >= nums.front()) {
            auto it = std::lower_bound(nums.begin(), nums.begin() + pos_min, target);
            int pos_insert_target = std::distance(nums.begin(), it);
            if (it == nums.end() || *it != target) {
                return -1;
            }
            return pos_insert_target;
        } else {
            // std::lower_bound()は`first <= last`でないと未定義動作なので，弾いておく
            if (it_min == nums.end()) {
                return -1;
            }
            
            auto it = std::lower_bound(nums.begin() + pos_min, nums.end(), target);
            int pos_insert_target = std::distance(nums.begin(), it);
            if (it == nums.end() || *it != target) {
                return -1;
            }
            return pos_insert_target;
        }
    }
};
```

---
ちなみにif-elseの分岐のなかでreturnまで行ってしまうのと，共通部分は括ってif-elseの外に出すのはどちらが読みやすいと感じますか？個人的には中に書いてしまった方が読む時は楽な気がしますが．ご意見お待ちしております：
```cpp
if (target >= nums.front()) {
    auto it = std::lower_bound(nums.begin(), nums.begin() + pos_min, target);
    int pos_insert_target = std::distance(nums.begin(), it);
    if (it == nums.end() || *it != target) {
        return -1;
    }
    return pos_insert_target;
} else {
    // std::lower_bound()は`first <= last`でないと未定義動作なので，弾いておく
    if (it_min == nums.end()) {
        return -1;
    }
    
    auto it = std::lower_bound(nums.begin() + pos_min, nums.end(), target);
    int pos_insert_target = std::distance(nums.begin(), it);
    if (it == nums.end() || *it != target) {
        return -1;
    }
    return pos_insert_target;
}
```
or
```cpp
if (target >= nums.front()) {
    auto it = std::lower_bound(nums.begin(), nums.begin() + pos_min, target);
} else {
    // std::lower_bound()は`first <= last`でないと未定義動作なので，弾いておく
    if (it_min == nums.end()) {
        return -1;
    }
    auto it = std::lower_bound(nums.begin() + pos_min, nums.end(), target);
}
int pos_insert_target = std::distance(nums.begin(), it);
if (it == nums.end() || *it != target) {
    return -1;
}
return pos_insert_target;
```

---

過去に解いたものが[PastCode](#PastCode)．
#### PastCode
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int i_rotate = GetRotationIndex(nums, nums[0], 1, nums.size());
        
        if (nums[0] == target) {
            return 0;
        }
        if (nums[0] < target) {
            return GetIndex(nums, target, 0, i_rotate);
        } else {
            return GetIndex(nums, target, i_rotate, nums.size());
        }
    }
private: 
    int GetRotationIndex(vector<int>& nums, int target, int first, int last) {
        int center = (first + last) / 2;
        while (first < last && center < nums.size()) {
            if (nums[center] == target) {
                return center;
            } else if (nums[center] < target) {
                last = center;
            } else if (nums[center] > target) {
                first = center + 1;
            }
            center = (first + last) / 2;
        }
        return center;
    }

    int GetIndex(vector<int>& nums, int target, int first, int last) {
        int center = (first + last) / 2;
        while (first < last) {
            if (nums[center] == target) {
                return center;
            } else if (nums[center] < target) {
                first = center + 1;
            } else if (nums[center] > target) {
                last = center;
            }
            center = (first + last) / 2;
        }
        return -1;
    }
};
```

# Step 2

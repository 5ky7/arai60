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
どうでもいいけど，Javaの`Arrays.binarySearch`ってターゲットと同じ数値が見つからなかったらビット反転したものが返ってくるんだ([cf](https://docs.oracle.com/javase/jp/8/docs/api/java/util/Arrays.html#binarySearch-byte:A-byte-))．

---

[区間は何を意味するか](https://github.com/seal-azarashi/leetcode/pull/38#discussion_r1836463140)．
- `first`: `nums[first - 1]`より手前には`target`より小さいものしかない．
- `last`: `nums[last]`以降には`target`より大きいものしかない．

---

入力配列に重複要素があった時に，何番目のindexを返すのか（[cf](https://discord.com/channels/1084280443945353267/1227464441235509308/1229628764443643935)）．
今のコードだと左端とは限らないね．種類としてはまず2つに大別できる：
- とりあえずtargetと同じ値があるか探す
- target以上の値で一番indexの小さいものを探す


「lower_boundの実装例を見てみると，探索範囲を半分にしていって，範囲の長さが0になったら終わり」という構造だった．イテレータで書いていたが，indexで書くと以下の[Code4](#Code4)．
- `first`: `nums[first]`より手前には`target`より小さいものしかない．
- `last`: `nums[last]`以降には`target`以上のものしかない．

これで`target`以上のものの中で一番indexが小さいもの，を得られる．

#### Code4
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int first = 0;
        int last = nums.size();
        while (first < last) {
            int mid = first + (last - first) / 2;

            if (nums[mid] < target) {
                first = mid + 1;
                continue;
            } else {
                last = mid;
            }
        }
        return first;
    }
};
```

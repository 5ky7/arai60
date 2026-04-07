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

まあそもそもbool型に変換しない方がわかりやすいかもしれない．

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
[見たことない方法](https://github.com/Yoshiki-Iwasa/Arai60/pull/36/changes#r1712955053)があったので自分なりに整理．
```python
class Solution:
    def search(self, nums: List[int], target: int) -> int:
        def priority(x):
            return (x <= nums[-1], target <= x)
        i = bisect_left(nums, priority(target), key=priority)
        if nums[i] != target:
            return -1
        return i
```
1. `key`パラメータが`None`でない場合，`bisect_left(a, x, key=key)`は`key(a[i])`と`x`を比較して二分探索を行う．
2. Pythonではタプルは辞書順比較される：`(False, False) < (False, True) < (True, False) < (True, True)`
3. `Priority`関数の意味は以下の通り
    - まず`x <= nums[-1]`は，`x`があるとしたら`nums`の前半(False)か，後半(True)かを示す．
    - 次に`target <= x`は，`x`があるとしたら`target`の入る箇所より右(True)か，左(False)かを示す
4. この仕組みにより`priority()`を通して`nums`を見ると，`nums`が"単調増加列(= 2分探索の前提条件)"になっている：
    - `priotity(nums[i])`の返り値は`(nums[i] <= nums[-1], target <= nums[i])`．
       - 第1成分はnumsの前半でFalse, 後半でTrue．この時点で第1成分について単調増加．
       - タプルは辞書順なので第1成分がそれぞれFalse, Trueになる範囲で単調増加なら，`[priority(nums[0]), ... priority(nums[-1])]`全体もは単調増加列と言える．そしてrotateの定義よりそれぞれの範囲の中では単調増加．
5. `bisect_left(nums, priority(target), key=priority)`は何をしているのか？
   - priorityを通してnumsを見た時に，priorityを通したtargetが挿入できる位置で一番0に近い位置を返している．つまり：
       - priorityの返り値の第1成分のおかげで以下のことが成立する
           - まず`target <= nums[-1]`なら，priority(target)の第1成分は`True`．`bisect_left`の返り値は`nums`の中で`priority(nums[i]) == True`のどこかになる
           - 次に`target > nums[-1]`なら，priority(target)の第1成分は`False`．`bisect_left`の返り値は`nums`の中で`priority(nums[i]) == False`のどこかになる
       - priorityの返り値の第2成分のおかげで以下のことが成立する
           - 座標圧縮的に，単調増加なnumsの部分列をFalse, Trueの列にmappingする．この時bisect_leftを実行することで普通の二分探索ができる．
           - つまり，実は`return (x <= nums[-1], target <= x)`は`return (x <= nums[-1], x)`でも良い！（どうせそのままでも増加列になっており，この部分に二分探索を適用すれば良いから）
   - というわけで，bisect_leftを2回やっているようなものなのである．1回目のbisect_leftは[Code1](#Code1)の`int pos_min = FindMinIndex(nums);`に対応し，2回目のbisect_leftは[Code1](#Code1)の`if ... else ...`内の`lower_bound`である．
  
C++で書いたら以下のようになったが，もっと綺麗に書けそう．pythonと違ってkeyの設定ができないから，numsをtransformするという方針をとった（pythonならこんなことしなくてもkeyに関数を入れておけば自動的に比較時にkeyを適用してから比較する，ということになる）．
#### Code3
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        auto priority = [&](int x) -> tuple<bool, bool> {
            return std::make_tuple(x <= nums.back(), target <= x);
        };
        vector<std::tuple<bool, bool>> nums_transformed(nums.size());
        std::transform(nums.begin(), nums.end(), nums_transformed.begin(), priority);
        auto it = std::lower_bound(nums_transformed.begin(), nums_transformed.end(), priority(target));
        int it_index = std::distance(nums_transformed.begin(), it);
        if (it == nums_transformed.end() || nums[it_index] != target) { 
            return -1;
        }
        return it_index;
    }
};
```

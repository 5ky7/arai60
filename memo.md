[153. Find Minimum in Rotated Sorted Array](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/)
# Step 1
[Code1](#Code1)．15分くらい．`first = 1`の初期値からして，`nums.size() == 1`の時はどうしよう，とか色々考えてたらややこしくなり，時間がかかってしまった．
#### Code1
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
        // targetより大きい数たちと，target以下の数たちの境界（つまり，target以下の数の最小）を探す．
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
せっかくなので閉区間のものも書いてみる．
#### Code2
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        int target = nums.front();
        int first = 1;
        int last = nums.size() - 1;
        // targetより大きい数たちと，target以下の数たちの境界（target以下の数の最小）を探す．
        while (first <= last) {
            int mid = first + (last - first) / 2;
            if (nums[mid] > target) {
                first = mid + 1;
            } else {
                last = mid - 1;
            }
        }
        // 存在すればfirstがそこに到達する．
        // 存在しない場合はfirstはnums.end()の位置に到達する．
        return first < nums.size() ? nums[first] : nums[0];
    }
};
```

別に`nums.front()`でなく`nums.back()`を基準にしても構わない．こちらも弊区間でやってみる．`int last = nums.size() - 2;`が不安になるが，
- `nums.size() == 1` -> while条件`first <= last`を抜けてそのまま`first`を出力
- `nums.size() == 2` -> `first == last`から始まり，`mid == first`であるので，
    - `nums[0] > nums[1]`　-> 
#### Code3
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        int target = nums.back();
        int first = 0;
        int last = nums.size() - 2;
        // targetより大きい数たちと，target以下の数たちの境界（target以下の数のうちindex最小のもの）を探す．
        while (first <= last) {
            int mid = first + (last - first) / 2;
            if (nums[mid] > target) {
                first = mid + 1;
            } else {
                last = mid - 1;
            }
        }
        // 存在すればfirstがそこに到達する．
        // 存在しない場合はfirstはnums.end()の位置に到達する．
        return first < nums.size() ? nums[first] : nums[0];
    }
};
```

---

`nums`を`nums[0]`より大きいか小さいかでTrueとFalseにmapして探す，という考え方が面白かった（[cf](https://discord.com/channels/1084280443945353267/1230079550923341835/1235694567085576275)）．
```py
    def findMin(self, nums: List[int]) -> int:
        return nums[bisect_left(nums, True, key=lambda x: x <= nums[-1])]
        return nums[bisect_left(nums, True, key=partial(ge, nums[-1]))]
        return nums[bisect_right(nums, False, key=lambda x: x < nums[0]) - len(nums)]
```
[`bisect_{left, right}`の挙動](https://docs.python.org/ja/3.14/library/bisect.html)をもとに考えると，それぞれの`return`文で得られるindex:`i`の性質は，
- `nums[-1]`以下の要素の中で一番indexが小さいもの = `nums[-1]`以下の領域と`nums[-1]`より大きい領域の境界．
- 上に同じ．`partial(ge, nums[-1])`は`lambda x: x <= nums[-1]`と等価なので．
- `nums[0]`より大きい要素の中で一番indexが大きいものの，一つ右隣 = `nums[0]`以下の領域と`nums[0]`より大きい領域の境界

ということになる．今回の問題だと`nums`に重複がないからどれも同じ．

なお`- len(nums)`は，意図としては`+ 1`をしたいのだが，そうすると`bisect_right`の返り値が`nums`の終端だと範囲外アクセスになる．そこで`- len(nums)`だと範囲外にならずに実質`+ 1`の意図が実現可能．


---

> 他に、自主性みたいなものがないようには思っていて、変数の意味は書いた人が決めるんですよ。
その決め方がいくつかあるんですが、それのキメラのような話をしているように見えますねえ。

これ，数学を教えていて「この解き方であってますか」と質問する生徒に同じことを思ったことがある．

---

Early returnをすれば必ず早くなるわけではない（[cf](https://discord.com/channels/1084280443945353267/1245404801177616394/1308054647679877192)）．分岐が入ることと，どれだけループや再帰を減らせるかとのトレードオフ．

---

二分探索を読むということ．[このコード](https://github.com/takuya576/leetcode/pull/2#discussion_r2049272458)とその直後のコードの違い．
- 一つ目
    - `right`: これより右には`target (== nums[0])`より小さい数しかないよ
    - `left`: これより左には`target`以上の数しかないよ
    - ループを抜けた後の`left`(> `right`である): `target (== nums[0])`より小さい数の最小index -> そんなものがないならout of indexになるので`% len(nums)`が必要
- 二つ目
    - `right`: これより右には`target (== nums[-1])`以下の数しかないよ
    - `left`: これより左には`target`より大きい数しかないよ
    - ループを抜けた後の`left`(> `right`である): `target (== nums[-1])`以下の数の最小のindex -> 少なくともtarget自身(== nums[0])は候補になるのでout of indexにならない．

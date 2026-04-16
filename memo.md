# Step 1
- `nums1`の要素を全て`set`か`unordered_set`に移した後，`nums2`の各要素がそのコンテナに入っているか確かめれば良い．最悪時間計算量は`N = max(nums1.size(),nums2.size())`として`set`ならO(N), `unordered_set`ならO(N log N)，空間計算量はどちらもO(N)
- `nums1`と`nums2`のどっちを`set`に入れてどっちをlook upに使うか→`insert()`と`contains()`の計算量の大小，および演算の回数できまる
  - `unordered_set`では`insert()`, `contains()`ともに平均O(1)
  - `set`では`では`insert()`, `contains()`ともに平均O(log N)
  - また，nums1とnums2でサイズの大小に対する情報（nums1の方が大きいなど）はない
  - よって**今回はどっちでも良い**ので`nums1`をmapに入れるようにする
    - サイズを取得して場合わけする方法も考えられる．
- `set`か`unordered_set`のどちらを使うか
  - O(1)とO(log N)だが，今回`length()`は1000以下，log_2 1000 ~ 10で小さい．したがって実質的な計算量はほぼ変わらない
  - また，Nが特別大きいわけでなければ実際の計算時間はこの2つで対して変わらず，むしろメモリ使用量のオーバーヘッドを考えると `set`を使うべきとのこと（[参照](https://groups.google.com/a/chromium.org/g/chromium-dev/c/rdxOHKzQmRY))
  - なのでここでは**setを採用**
- 実装してテストを走らせてみたら，重複をそのまま追加していた（以下の1つ目のコード）ので，`set`を`map`にしてvalueの値で重複を判断（以下の2つ目のコード）．
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        set<int> set_nums1;
        for (int i : nums1) {
            set_nums1.insert(i);
        }

        vector<int> nums_duplicated;
        for (int j : nums2) {
            if (set_nums1.contains(j)) {
                nums_duplicated.push_back(j);
            }
        }

        return nums_duplicated;
    }
};
```

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // nums1の各要素をmapに格納
        map<int, bool> set_nums1; // {num, first_appear}の形．nums2のforでfirst_appear == trueのnumのみ追加していく．
        for (int i : nums1) {
            set_nums1.insert({i, true});
        }

        // nums2とmap_nums1重複をチェックして追加
        vector<int> nums_duplicated;
        for (int j : nums2) {
            if (set_nums1.contains(j) && set_nums1[j]) {
                nums_duplicated.push_back(j);
                set_nums1[j] = false;
            }
        }

        return nums_duplicated;
    }
};
```
    
# Step 2
* ソート済みなら`nums1`,`nums2`の最初から見ていくことが可能．ということでTwo pointersで解いてみる（下のコード）．`N = max(nums1.size(),nums2.size())`として最悪時間計算量はソートがボトルネックでO(N log N), 空間計算量はO(N)．
  * indexを変数に持つのと，イテレータを変数に持つのってどっちが見やすいのだろうか．意見お待ちしております．
    * 今回はindexが直接必要になることはないので，イテレータの方が良いかもしれない．（下の下のコード）
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());

        vector<int> solution;
        int i = 0;
        int j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] == nums2[j]) {
                solution.push_back(nums1[i]);
                i = NextIndex(nums1, i);
                continue;
            }
            if (nums1[i] < nums2[j]) {
                i = NextIndex(nums1, i);
            } else {
                j = NextIndex(nums2, j);
            }
        }
        return solution;
    }
private:
    int NextIndex(vector<int>& nums, int i) {
        while (i < nums.size() - 1 && nums[i] == nums[i+1]) {
            ++i;
        }
        return ++i;
    }
};
```
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());

        vector<int> solution;
        auto nums1_pointer = nums1.begin();
        auto nums2_pointer = nums2.begin();
        while (nums1_pointer != nums1.end() && nums2_pointer != nums2.end()) {
            if (*nums1_pointer == *nums2_pointer) {
                solution.push_back(*nums1_pointer);
                NextIndex(nums1, nums1_pointer);
                continue;
            }
            if (*nums1_pointer < *nums2_pointer) {
                NextIndex(nums1, nums1_pointer);
            } else {
                NextIndex(nums2, nums2_pointer);
            }
        }
        return solution;
    }
private:
    void NextIndex(vector<int>& nums, auto& nums_pointer) {
        while (nums_pointer != nums.end() - 1 && *(nums_pointer) == *(nums_pointer + 1)) {
            ++nums_pointer;
        }
        ++nums_pointer;
        return;
    }
};
```

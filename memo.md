# Step 1
- `nums1`の要素を全て`set`か`unordered_set`に移した後，`nums2`の各要素がそのコンテナに入っているか確かめれば良い．最悪時間計算量，空間計算量はO(N)
- `nums1`と`nums2`のどっちを`set`に入れてどっちをlook upに使うか→`insert()`と`contains()`の計算量の大小できまる
  - `unordered_set`では`insert()`, `contains()`ともに平均O(1)
  - `set`では`では`insert()`, `contains()`ともに平均O(log N)
  - よって**今回はどっちでも良い**ので`nums1`をmapに入れるようにする
- `set`か`unordered_set`のどちらを使うか
  - O(1)とO(log N)だが，今回`length()`は1000以下，log_2 1000 ~ 10で小さい．したがって実質的な計算量はほぼ変わらない
  - また，Nが特別大きいわけでなければ実際の計算時間はこの2つで対して変わらず，むしろメモリ使用量のオーバーヘッドを考えると `set`を使うべきとのこと（[参照](https://groups.google.com/a/chromium.org/g/chromium-dev/c/rdxOHKzQmRY)
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

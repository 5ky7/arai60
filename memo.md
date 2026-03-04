[213. House Robber II](https://leetcode.com/problems/house-robber-ii/description/?envType=problem-list-v2&envId=n97t3qfj)
# Step 1
いつだか解いたものが[Code1](#Code1)．

時間が経ったので解き直してみる．家が円状に並んでいるということは，最初の家と最後の家が連結しているのでこの2つを同時に含むことはできない．ならばnums[0:-2]とnums[1:]のそれぞれにHouse Robberの手法を適用し，最大値を取れば良い．ただし`vector<int>`を作成し直すのは時間がかかりそうなので`span`を利用．[Code2](#Code2)


#### Code1
```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int i_max = nums.size() - 1;
        if (i_max == 0) {
            return nums[0];
        }
        if (i_max == 1) {
            return max(nums[0], nums[1]);
        }
        return max(robHelper(nums, 0, i_max - 1), robHelper(nums, 1, i_max));
    }

private:
    int robHelper(vector<int>& nums, int left, int right) {
        if (left >= right) {
            return nums[left];
        }
        if (left + 1 == right) {
            return max(nums[left], nums[right]);
        }

        int len_range = right - left + 1;
        vector<int> max_robbery(len_range);
        max_robbery[0] = nums[left];
        max_robbery[1] = nums[left + 1];
        for (int i = 2; i < len_range; ++i) {
            max_robbery[i] = max(max_robbery[i - 2] + nums[left + i], max_robbery[i - 1]);
        }
        
        return *max_element(max_robbery.begin(), max_robbery.end());
    }
};
```

#### Code2
```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0];
        }
        
        std::span<const int> nums_span(nums);
        int max_return_without_last = MaxReturnArrangedInLine(nums_span.subspan(0,nums.size() - 1));
        int max_return_without_first = MaxReturnArrangedInLine(nums_span.subspan(1,nums.size() - 1));
        return std::max(max_return_without_last, max_return_without_first);
    }

private:
    int MaxReturnArrangedInLine(std::span<const int> nums) {
        if (nums.empty()) {
            return 0;
        }

        int prev_prev_max = 0;
        int prev_max = 0;
        int max_amount = 0;
        for (int ith_house = 1; ith_house <= nums.size(); ++ith_house) {
            max_amount = std::max(prev_max, prev_prev_max + nums[ith_house - 1]);
            prev_prev_max = std::max(prev_prev_max, prev_max);
            prev_max = std::max(prev_max, max_amount);
        }

        return max_amount;
    }
};
```

# Step 2
- `span`のコンストラクタは`std::span<const int> nums_span_without_first(nums.begin(), nums.end() - 1)`のようにイテレータで範囲指定も可能．[cf](https://github.com/potrue/leetcode/pull/36/changes#diff-d3cedbf7511b9e154887733e0eb21d3ff29647725d96d4c901b62042cd7a32b7R34)
    - ここで`nums.end() - 1`を見て「空配列だとエラー吐きそうだな」と思えるようになりたい．

---

ラムダ式のメリット・デメリット．[cf1](https://github.com/potrue/leetcode/pull/36/changes#r2253909474)，[cf2](https://ttsuki.github.io/styleguide/cppguide.ja.html#Lambda_expressions)．

メリットは，
- 関数オブジェクトを作成するときに便利．特に引数として無名関数を渡したいときに引数に直接書き込める．
- キャプチャが便利．メンバ関数化すると引数が冗長になる恐れがある．
    - 具体的に上記2つが役に立つ例として，並列計算フレームワークで計算ロジックをネストさせたい場合が挙げられる．
```cpp
std::vector<double> a(n), b(n), c(n);

parallel_for(0, n, [&](int i) {
    c[i] = a[i] + b[i]; // キャプチャが便利．
});
```
```cpp
parallel_for(0, N, [&](int i) {
    parallel_for(0, N, [&](int j) { // 無名関数をネストさせられる
        double sum = 0;
        for (int k = 0; k < N; k++) {
            sum += A[i][k] * B[k][j];
        }
        C[i][j] = sum;
    });
});
```

デメリットは，
- 長くなると目線の上下動が大きくなり，読むのに疲れる
- キャプチャされたポインタと関数内ローカル変数の関係付けによるダングリングポインタの危険性
    - ダングリングポインタ: 無効になったメモリ領域を指すポインタ
    - キャプチャは実のところ変数宣言であることに注意する．型は明示されないが，実質的にautoであると考えて良い．
- 関数名がないと，ロジックが複雑になると理解しにくい．

# Step 1
* 過去に解いたものが[Code1](#Code1).
* 時間が経ったので解き直してみたのが[Code2](#Code2).所要時間28分．時間計算量O(n^2), 空間計算量O(1)．
  * 一旦「連続な部分列」だと勘違いして解いた．つまり入力`[10,9,2,5,3,7,101,18]`に対して`[2, 3, 7, 101]`でなくて`[3, 7, 101]`が出力になるように書いてしまった．
  * 書き直そうとしたが，`[1, 5, 2, 6, 3, 7, 4, 5]`に対して面倒臭そうと思った．つまり部分列を大量に保持して比べる（あるいは総当たりする）べきか？と思った
  * しかし一つの部分列を持っていれば良いことに気づいた．大量に保持せずとも部分列の一部を**最も効率的（つまり含まれる数がより小さい）な部分列**になるように更新していけば，もしその部分列がもとの部分列より長くなるなら勝手に全体が更新されるし，そうでないなら一部のみ更新されるが長さの情報は保持されたままになることに気づいた．

### Code1
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> longest_subseq;
        for (int num : nums) {
            if (longest_subseq.empty() || num > longest_subseq.back()) {
                longest_subseq.push_back(num);
                continue;
            }
            for (int i = 0; i < longest_subseq.size(); ++i) {
                if (num <= longest_subseq[i]) {
                    longest_subseq[i] = num;
                    break;
                }
            }
        }
        return longest_subseq.size();
    }
};
```

### Code2
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        vector<int> increasing_subsequence(1, nums.front());
        int max_len = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if (increasing_subsequence.back() < nums[i]) {
                increasing_subsequence.push_back(nums[i]);
                continue;
            }
            UpdateSubsequence(nums[i], increasing_subsequence);
        }
        return increasing_subsequence.size();
    }

private:
    void UpdateSubsequence(int new_num, vector<int>& nums) {
        // numsの要素でnew_numより以下の数のうち，最大のものをnew_numで置き換える
        for (int i = 0; i < nums.size(); ++i) {
            if (new_num <= nums[i]) {
                nums[i] = new_num;
                break;
            }
        }
        return;
    }
};
// [1, 5, 2, 6, 3, 7, 4, 5]
// [1, 2, 3, 4, 5]

// [10,9,2,5,3,7,101,18]
// [2, 3, 7, 18]
```

# Step 2
* forループで総当たりするときはDP的にやると良い（[cf](https://github.com/hayashi-ay/leetcode/blob/hayashi-ay-patch-16/300.%20Longest%20Increasing%20Subsequence.md)）
* [Code2](#Code2)は時間計算量O(n^2)としたが，[二分探索でO(NlogN)にできる](https://github.com/haniwachann/leetcode/pull/5#discussion_r1859480859)．[Code3](#Code3)
  * この選択肢は頭の中に思い浮かべておきたかった．「探す」の選択肢として「特有の構造（今回なら単調増加性）を利用するか，総当たりか」の分岐を持っておきたい．
* セグメントツリーについて．
  * [このコード](https://github.com/hayashi-ay/leetcode/pull/27/changes#diff-b7fbb0dce1473afc0264185268f1a1ef6d682a3a8c997d43bc8bdd636a66ce4aR207)をみるも，何をやっているのか理解できなかった．コードを書いてみて([Code4(#Code4)])，手で挙動を確認してみたが，いまいち理解しきれていない．
    * セグ木が，配列のとある範囲における最大値を効率的に返すことはわかった．
    * セグ木の中に保持する配列は`nums`（あるいはそれを座標圧縮したもの）ではないこともわかった．
    * numsを座標圧縮した上で，numsの各要素`num`に対して`[0,num]`の範囲を調べているのもわかったが，なぜこれを調べるのかはわからない．
      * 座標圧縮したおかげで調べる範囲が無駄に広くなるのを防いでいそう？
    * セグ木のアップデートのたびに最大長がセグ木に保持されていってそうなのはわかったが，更新位置が`i`になるのはよくわからない．というかセグ木が何をどんな論理で保持しているのかもわからない．
    * セグ木に保持しているのは何だろうか？
      * `nums`の要素数と同じ個数の実質的なデータ領域があり，残りは効率的に最大値を求めるためのデータ用の領域
      * `nums`の要素が全て相異なるとする．`nums`の要素`nums[i] == num`に注目した時，`nums[0]`から`nums[i]`までを使って組み立てられる部分列の最長を返している？
      * しかしどういうロジックなのかがわからない．セグ木の中にはもはや`nums`の順番は保持されていないのに，部分列は`nums`の要素の順番が大事な役割を果たしそう．なぜセグ木から部分列の要素が取り出せる？
  * [こっち](https://github.com/thonda28/leetcode/pull/16/changes#diff-5d85004121fdaf34abe9af172f266e7f57f39e2165ac80e3ecbd849d1831c141R12)はセグ木のサイズを2の冪乗にしたバージョン．セグ機の実装自体は多少わかりやすいかもだが，私の疑問「なぜセグ木によってその目的を達成できるのか」はまだわかっていない．
  * [この説明](https://discord.com/channels/1084280443945353267/1200089668901937312/1209563502407065602)でいうところの，「k+1番目に進むときは、『セグメント木を使うと、-10^4 から nums[k+1] -1 で終わるシーケンスの最大長が求められるので、それに1を足します。』」の部分がわからない．
    * 座標圧縮していなかったら，「nums[k+1] -1で終わるシーケンス」の情報はいつ追加されるのか？つまり，nums[0]からnums[k]までの処理のなかでnums[k+1] -1という数値が出てくるとは限らないのに，なぜその数値に対応する情報がセグ木に含まれているのか？
    * 座標圧縮していても，`nums`は昇順とかではないので，やはりnums[0]からnums[k]までの処理のなかでnums[k+1] -1という数値が出てくるとは限らないと思う．


### Code3
```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        vector<int> increasing_subsequence(1, nums.front());
        int max_len = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if (increasing_subsequence.back() < nums[i]) {
                increasing_subsequence.push_back(nums[i]);
                continue;
            }
            UpdateSubsequence(nums[i], increasing_subsequence);
        }
        return increasing_subsequence.size();
    }

private:
    void UpdateSubsequence(int new_num, vector<int>& nums) {
        // numsの要素でnew_numより小さい数のうち，最大のものをnew_numで置き換える
        auto it = std::lower_bound(nums.begin(), nums.end(), new_num);
        *it = new_num;
        return;
    }
};
```
```cpp

#include <vector>
#include <algorithm>

using namespace std;

class Solution {
 public:
  int lengthOfLIS(vector<int>& nums) {
    vector<int> compressed_nums = Compress(nums);
    SegmentTree tree = SegmentTree(compressed_nums);

    for (int num : compressed_nums) {
      int best = tree.GetMax(0, num);
      tree.Update(num, best + 1);
    }

    return tree.GetMax(
        0, *std::max_element(compressed_nums.begin(), compressed_nums.end()) + 1);
  }

  class SegmentTree {
   private:
    int size_;
    vector<int> tree_and_nums_;

   public:
    SegmentTree(const vector<int>& nums)
        : size_(*std::max_element(nums.begin(), nums.end()) + 1),
          tree_and_nums_(2 * size_, 0) {}

    int GetMax(int left, int right) {
      int result = 0;
      left += size_;
      right += size_;
      while (left < right) {
        if (left % 2 == 1) {
          result = std::max(result, tree_and_nums_[left]);
          ++left;
        }
        if (right % 2 == 1) {
          --right;
          result = std::max(result, tree_and_nums_[right]);
        }
        left /= 2;
        right /= 2;
      }
      return result;
    };

    void Update(int pos, int value) {
      pos += size_;
      while (pos > 0) {
        if (tree_and_nums_[pos] >= value) {
          return;
        }
        tree_and_nums_[pos] = value;
        pos /= 2;
      }
    };
  };

 private:
  vector<int> Compress(const vector<int>& nums) {
    vector<int> sorted_unique_nums = nums;
    std::sort(sorted_unique_nums.begin(), sorted_unique_nums.end());
    sorted_unique_nums.erase(
        unique(sorted_unique_nums.begin(), sorted_unique_nums.end()),
        sorted_unique_nums.end());

    vector<int> compressed_nums(nums.size());
    for (int i = 0; i < nums.size(); ++i) {
      auto rank_ith_num = lower_bound(sorted_unique_nums.begin(),
                                      sorted_unique_nums.end(), nums[i]) -
                          sorted_unique_nums.begin();
      compressed_nums[i] = static_cast<int>(rank_ith_num);
    }
    return compressed_nums;
  }
};
```

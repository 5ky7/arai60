# Step 1
- いつだか解いたものが[Code1](#Code1)．
- 時間が経ったので解き直してみる．所要時間22分．
  - とりあえず総当たりを書いてみる．[Code2](#Code2)．5分程度
    - 時間計算量O(n^2), 空間計算量O(1)．
    - `nums`に負の数が含まれていることを忘れて`max_sum_subarray`の初期値を0にしてしまった．`numeric_limits<int>::min()`に修正するも，（当然）TLE．
  - 工夫を考える．
    - 一つ小さいサイズの問題の解を参照するのは不可能ぽい．`[2, -1, 3]`に対して`[2,1]`の解は`[2]`だから，ここから`[2,-1,3]`を構築するのは無理そう．
    - 連続部分列だから，ある`nums[i]`を部分列に含むことができるのは，`nums[i-1]`が部分列に含まれる時か，`nums[i]`が最初の要素となる時のみ．
    - ということは，一つ小さいサイズを参照するには，問題を変えて，「`nums[i]`を最後に含む部分列で最大のもの」を，「`nums[i-1]`を最後に含む部分列で和が最大のものと，`nums[i-1]`を含まないもの（＝空配列）」を比べれば良い．
    - この発想で書いたのが[Code3](#Code3)．
      - 時間計算量O(n), 空間計算量O(n)．
      - 空間計算量は最大値のみ保持しておけばO(1)に減らせる．減らした後のものをKadaneのアルゴリズムと呼ぶ．

### Code1
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        vector<int> max_length_with_itself(nums.size());
        max_length_with_itself[0] = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            max_length_with_itself[i] = nums[i];
            if (max_length_with_itself[i - 1] > 0) {
                max_length_with_itself[i] += max_length_with_itself[i - 1];
            }
        }

        return *max_element(max_length_with_itself.begin(), max_length_with_itself.end());
    }
};
```

### Code2
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int sum_subarray = 0;
        int max_sum_subarray = 0;
        for (int first = 0; first < nums.size(); ++first) {
            sum_subarray += nums[first];
            max_sum_subarray = std::max(max_sum_subarray, sum_subarray);
            for (int last = first + 1; last < nums.size(); ++last) {
                sum_subarray += nums[last];
                max_sum_subarray = std::max(max_sum_subarray, sum_subarray);
            }
            sum_subarray = 0;
        }
        return max_sum_subarray;
    }
};
```

### Code3
```cpp
#include <vector>
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        std::vector<int> max_subarray_with_itself(nums.size());
        max_subarray_with_itself[0] = nums[0];
        for (int i = 1; i < nums.size(); ++i) {
            max_subarray_with_itself[i] = nums[i];
            if (max_subarray_with_itself[i - 1] > 0) {
                max_subarray_with_itself[i] += max_subarray_with_itself[i - 1];
            }
        }

        return *max_element(max_subarray_with_itself.begin(), max_subarray_with_itself.end());
    }
};
```
# Step 2
- [O(n)解法の別の発想](https://discord.com/channels/1084280443945353267/1206101582861697046/1207518775851876362)を参考に書いたのが[Code4](#Code4)．
  - こっちは空間計算量がO(1)．
    - と思ったが，[Code3](#Code3)も`std::vector<int> max_subarray_with_itself`の代わりに最大値だけ保持しておけば空間計算量O(1)に減らせるね．
  - 言われてみれば確かにワンパスでいける．「今の累積和」，「それまでの累積和の最小値」があれば確かに「それまでの最大部分列の和」が求められて，一つ次のループで必要な「次の累積和」と「（範囲を一つ伸ばした）それまでの累積和の最小値」はそれぞれ次のループ内，今のループの最後で求められる．
  - 発想の違いはどこだろう？部分列の構築に注目（Step1の[Code3](#Code3)）するのか，数値に注目するか（[Code4](#Code4)）の違い？（そうではない気がする）
    - 少なくとも，Kadaneとこのやり方の根底が同じだと現時点では思えない．「部分列の和が0を下回ったらリセット」がKadaneの根底にありそうだが，このやり方は別の視点のように思う．
      - [こちら](https://discord.com/channels/1084280443945353267/1227073733844406343/1243146879777243186)を参考にすると関連が見える．
      - 参照先の`new_prefix_sum - new_min_prefix_sum`の計算に出てくる`old_prefix_sum - min(old_min_prefix_sum, old_prefix_sum)`がいい感じに`max(old_prefix_sum - old_min_prefix_sum, 0)`になるのが面白いポイント．
      - あくまで現在の`nums[i]`を含む部分和の最大値が`prefix_sum - min_prefix_sum`であると捉える．計算すると，結果的に空配列かどうかの場合分けになる．
    - [これ](https://github.com/olsen-blue/Arai60/pull/32#discussion_r1962010150)もわかりやすい．ここまでくるとKadaneと直接関連が見える．
  - [こう考えた](https://discord.com/channels/1084280443945353267/1206101582861697046/1208414507735453747)らしい．
    - 選択肢を増やすこと，そのために今思いつく方法をいじることである面で（例えば計算量，素直さなど）よりよくできないか考えること，が大事そう．
    - その上で選択肢を比較検討すること．
- [Code3](#Code3)の更新則部分は`max`を使って`max_subarray_with_itself[i] = nums[i] + std::max(max_subarray_with_itself[i - 1], 0);`と書いた方がわかりやすいか？if文の方が「和が0より大きい」ということが強調できて良いか？
- [番兵の利用](https://discord.com/channels/1084280443945353267/1337642831824814192/1362687759474757793)による初期化．
  - Code4の`max_sum_of_subarray`の初期値を`nums[0]`から`numeric_limits<int>::min()`に変えた方が良いか．
  - どっちで初期化しても，入力制約の無視や拡張に対しては問題なく対応できそう．
  - 一方で更新則の美しさというか，`nums[0]`を含む部分列を見るときに初めて`max_sum_of_subarray`を更新したいと考えると`numeric_limits`で初期化した方がいいかも．
    - つまり`nums[0]`を最初から入れておくと，2回検査することになって気持ち悪い．
- テストケースを考えてみる．[参考](https://github.com/Mike0121/LeetCode/pull/51#discussion_r1969030728)．
  - 入力が全て正: `nums = [1, 2, 5, 1]` → Output: 9
  - 入力が全て負: `nums = [-1, -2, -5, -1]` -> Output: -1
  - 入力が正負まじり: `nums = [1, -3, 3, -2, 1]` -> Output: 3
  - 入力が極端なケース
    - `nums = []` (今回は制約で弾かれる．各コードでは返り値は0にしたが，これで良いのかは（空配列を部分列と解釈しない以上）怪しい．エラーを返すようにしても良いかも．）
    - `nums = [1]`
    - `nums = [-1]`
    - `nums = [0]`
    - `nums = [numeric_limits<int>::min()]`
    - 要素は全て1で，`nums.size() = 10000000`

### Code4
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }

        int cumulative_sum = 0;
        int min_cumulative_sum = 0;
        int max_sum_of_subarray = nums[0]; 
        for (int num : nums) {
            cumulative_sum += num;
            max_sum_of_subarray = std::max(max_sum_of_subarray, cumulative_sum - min_cumulative_sum);
            min_cumulative_sum = std::min(min_cumulative_sum, cumulative_sum);
        }

        return max_sum_of_subarray;
    }
};
```

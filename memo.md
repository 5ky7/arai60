[121. Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/)
# Step 1
いつだか解いたものが[Code1](#Code1)と[Code2](#Code2)．それぞれ[Code5](#Code5)と[Code3](#Code3)に対応しているが，後者に関しては[Code2](#Code2)の方が見やすいかもしれない．[Code3](#Code3)の`max_price`はない方がスッキリしている．`max_price`は更新に一度使われたらその後使われないのだから．

時間が経ったので解き直してみる．[Code3](#Code3)，10分くらい．
これ$`\max_i\, (\mathrm{nums[i]} - \min_{j<i}\, \mathrm{nums[j]})`$だから，[53. Maximum Subarray](https://github.com/5ky7/arai60/pull/33)と対応している．あっちの入力の累積和を入力にした問題が，ちょうど今回の問題と対応している．

逆に，累積和から差分を取ることで[53. Maximum Subarray](https://github.com/5ky7/arai60/pull/33)でやったような解き方ができる．[Code4](#Code4)がWAで，`prices.size() == 2`の時のエラーを考えられていなかった．これを修正して[Code5](#Code5)．

###### Code1
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() == 1) return 0;

        vector<int> dif_prices(prices.size() - 1);
        for (int i = 0; i < dif_prices.size(); ++i) {
            dif_prices[i] = prices[i + 1] - prices[i];
        }

        vector<int> max_subarray(dif_prices.size(), 0);
        max_subarray[0] = dif_prices[0];
        for (int i = 1; i < max_subarray.size(); ++i) {
            max_subarray[i] = std::max(max_subarray[i - 1] + dif_prices[i], dif_prices[i]);
        }

        return std::max(0, *std::max_element(max_subarray.begin(), max_subarray.end()));
    }
};
```

###### Code2
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int min_price = INT_MAX;
        int max_profit = 0;
        for (int i = 0; i < prices.size(); ++i) {
            if (prices[i] < min_price) {
                min_price = prices[i];
            }
            if (max_profit < prices[i] - min_price) {
                max_profit = prices[i] - min_price;
            }
        }
        return max_profit;
    }
};
```

###### Code3
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }

        int max_price = 0;
        int min_price_until_max_price = std::numeric_limits<int>::max();
        int max_return = 0;
        for (int current_price : prices) {
            max_price = std::max(max_price, current_price);
            max_return = std::max(max_return, max_price - min_price_until_max_price);
            if (current_price < min_price_until_max_price) {
                min_price_until_max_price = current_price;
                max_price = min_price_until_max_price;
            }
        }
        return max_return;
    }
};
```

###### Code4
```cpp
// Wrong Answer
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }

        std::vector<int> prices_diff(prices.size() - 1); 
        for (int i = 0; i < prices_diff.size(); ++i) {
            prices_diff[i] = prices[i + 1] - prices[i];
        }

        std::vector<int> max_return_ending_with(prices_diff.size());
        // max_return_ending_with[i] : prices[i + 1]で売るときのリターンの最大値．
        max_return_ending_with[0] = prices_diff[0];
        for (int i = 1; i < max_return_ending_with.size(); ++i) {
            max_return_ending_with[i] = prices_diff[i];
            if (max_return_ending_with[i - 1] > 0) {
                max_return_ending_with[i] += max_return_ending_with[i - 1];
            }
        }
        int max_return_with_buy = *max_element(max_return_ending_with.begin(), max_return_ending_with.end());
        return std::max(0, max_return_with_buy);
    }
};
```

###### Code5
```cpp
#include <vector>

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() < 2) {
            return 0;
        }

        std::vector<int> prices_diff(prices.size() - 1); 
        for (int i = 0; i < prices_diff.size(); ++i) {
            prices_diff[i] = prices[i + 1] - prices[i];
        }

        std::vector<int> max_return_ending_with(prices_diff.size());
        // max_return_ending_with[i] : prices[i + 1]で売るときのリターンの最大値．
        max_return_ending_with[0] = prices_diff[0];
        for (int i = 1; i < max_return_ending_with.size(); ++i) {
            max_return_ending_with[i] = prices_diff[i];
            if (max_return_ending_with[i - 1] > 0) {
                max_return_ending_with[i] += max_return_ending_with[i - 1];
            }
        }
        int max_return_with_buy = *max_element(max_return_ending_with.begin(), max_return_ending_with.end());
        return std::max(0, max_return_with_buy);
    }
};
```
# Step 2
テストケースを考えてみる．
- 要素数0 or 1
  - `[]`
  - `[1]`
- 要素数2
  - `[1,2]`
  - `[2,1]`
  - `[1,1]`
- 要素数3以上
  - `[1,1,1]`
  - `[1,3,2]`
  - `[2,1,3]`
  - `[1,2,4]`
  - `[4,2,1]`
  - `[1,3,4]`
  - `[4,3,1]`
- その他の例
  - `[5,2,4,1,7,6,1]`
 
---

scanlについて．
- [関数型っぽい](https://discord.com/channels/1084280443945353267/1233603535862628432/1290661484057329685)の意味
  - 今自分がscanlと同じ動作をしようとしたら，forを回す．実際上のコードでは全部そうしている．このforには`i`というループ変数を持ち，これを更新していく処理を含む．この`i`は状態を表していると言える．
  - 関数型では状態更新を扱わない．関数適用の連続で処理を行う．
    - 実際 $`\mathrm{scanl} \, (f) \,\, x_0 \,\, [x_1, x_2,\ldots,x_n]`$は
      - 入力$`[x_1, x_2,\ldots,x_n]`$に対して
      - 出力$`[x_0, f(x_0, x_1), f(f(x_0,x_1), x_2),\dots, ]`$を返す．
  - ここに*一時的な状態変数*や*その更新則*は含まれない．
  - 状態があるとしたら，関数の引数として渡す．上の例では$`x_i`$を関数$`f`$に渡している：$`f_i(\cdot) = f(f_{i-1}(\cdot),\, x_i)`$
    - これは$`今の状態 \to 次の状態`$という関数と捉えられる．
- メリットはいろいろあるみたいだが，今述べた話題に絞ると例えば高階関数の抽象化が挙げられる．
  - scanlに適切な関数を渡すことで，累積和やprefix maxが同じ形で書ける．
- 他のメリットとしては
  - 副作用がない（最小限に抑える）前提
    - 外部状態や実行順序によらない -> 安全性，並列生が高い．
  - 参照透過生（同じ入力に対して常に同じ出力; 状態変数を持たないためこれが実現）
    - 数学の関数と同等に扱える -> 解析がしやすいなど．

その帰結として，[ワンパス](https://discord.com/channels/1084280443945353267/1192728121644945439/1218818241636339722)で書くということができる．関数の連続適用なので．これをさらに潰す（必要なものだけ保持する）と[Code2](#Code2)になるわけだ．

C++には`std:inclusive_scan()`があるのでこれを用いて実装してみた([Code6](#Code6))が，あんまり綺麗じゃないように感じる．もっと良い書き方があるのか，あるいは私が関数型になれていないだけか．

###### Code6
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }

        std::vector<pair<int,int>> min_price_and_max_profit(prices.size());
        std::inclusive_scan(prices.begin(),
                            prices.end(),
                            min_price_and_max_profit.begin(),
                            [](pair<int, int> acc, int price) -> pair<int, int>{
                                auto [min_price_so_far, max_profit_so_far] = acc;
                                int max_profit = std::max(max_profit_so_far, price - min_price_so_far);
                                int min_price = std::min(min_price_so_far, price);
                                return {min_price, max_profit};
                            },
                            pair<int,int>(std::numeric_limits<int>::max(),0));
        
        return min_price_and_max_profit.back().second;
    }
};
```

複数の処理（`min_price`の取得，`max_profit`の取得など）をあえて分けて書き直してみた．[Code7](#Code7)．こっちの方が読みやすいかなー，微妙な感じ．
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }

        std::vector<int> min_prices(prices.size());
        std::inclusive_scan(prices.begin(),
                            prices.end(),
                            min_prices.begin(),
                            [](int min_so_far, int price) -> int{
                                return std::min(min_so_far, price);
                            });
        std::vector<int> max_profit_so_far(prices.size());
        std::transform(prices.begin(),
                        prices.end(),
                        min_prices.begin(),
                        max_profit_so_far.begin(),
                        [](int price, int min_price) -> int{
                            return price - min_price;
                        });

        return *std::max_element(max_profit_so_far.begin(), max_profit_so_far.end());
    }
};
```

---

[この辺](https://discord.com/channels/1084280443945353267/1262688866326941718/1345967955233607701)を参考に計算時間を求めてみる．[Code2](#Code2)を例に．
- CPUクロック周波数：最近のPCだと1GHz~5GHzくらい
- 命令：主要部はループで行われる以下の操作
  - 大小比較: 2回
  - 減算: 1~2回
  - 代入: 0~2回
  - if分岐: 2回
- 平均CPI
  - 大小比較:  1cycle/命令
  - 減算:     < 1cycle/命令
  - 代入:
    - レジスタで完結: 0~1cycle/命令
    - メモリアクセスあり: L1なら数cycle，L2以下だと10~10^3cycle/命令
    - 今回はほぼレジスタ上で完結しそう．int（4byte)が2つなので，L1もいらないかな．
  - if分岐: 予測が当たるかどうかでサイクルが変わる．ミスすると[平均16~20くらいのサイクルロス](https://www.7-cpu.com/cpu/Skylake.html?utm_source=chatgpt.com#:~:text=Branch%20misprediction%20penalty,mOp%20cache%20miss)が起こるらしい．ヒットすれば0~1cycle/命令とのことなので，確率1/2として10サイクル/命令程度と解釈
- ループあたりのサイクル数：以上の議論より平均で20cycleくらいか．
- ループ回数は`price.size() < 10^5`とのことなので，最悪でも以下の通り．

$$
実行時間 = \frac{サイクル数}{クロック周波数} 秒 \approx \frac{10^5 \times 20}{2 \times 10^9}秒 \approx  10^{-3}秒 = 1\mathrm{ms}
$$

あれ，なんか遅すぎないか？

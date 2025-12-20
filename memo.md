# Step 1
* いつだか解いた時のものが[Code1](#Code1)．
* 時間が経ったので解き直してみる．[Code2](#Code2)．間に用事が入ったため所要時間不明（30分くらい？）
  * 額の大きいコインから順に引いていけばいいじゃん，と思って[Code2](#Code2)を書いたがWrong Answer.
    * これだと`coins = [3, 998]`, `amount = 999`の時に`-1`を返してしまう．
  * これはボトムアップで最小枚数を計算していく方が良いと思い書いたのが[Code3](#Code3)．
    * ループ変数`i`に何も考えず`int`を用いたため，`if (i + coin <= amount)`の`i + coin`でオーバーフロー．`long longに変更`．
      * 条件文の`if (i <= amount - coin)`への変更も考えたが，可読性を考えるとindexとして`i + coin`が`amount`を超えない，という意味が分かりにくくなるため`i`の数値型を変更することにした．
      * こういうのは事前に頭でシミュレーションする時に気付けるようにしたい．
    * `min_num_coins`の初期値に何を用いるか．
      * 構成不可能性を示すために重要な数．候補は
        * `-1`
          * 最後に`return min_num_coins[amount]`とシンプルに返せる
          * `min_num_coins`の要素更新の条件分岐がめんどくさくなる
            * `-1`が入っていたら代入，そうでなければ数値比較を行う．[Code3](#Code3)では三項演算子を用いたが可読性は...
              * [三項演算子は賛否両論ある](https://docs.google.com/document/d/11HV35ADPo9QxJOpJQ24FcZvtvioli770WWdZZDaLOfg/edit?tab=t.0#heading=h.7zcdki273ah6)が，ネストが深くなるのがあまり好きでないため，また条件と評価式が対して複雑でもないため，今回は三項演算子を採用．
          * `amount`の想定する数が増えたときに書き換えるべき場所が少ない．`vector<int> min_num_coins(amount + 1, -1);`の`int`とforのループ変数の数値型をいじるのみ．
        * `INT_MAX`（選んだ数値型の最大値）
          * `min_num_coins`の要素更新の条件分岐が簡単．
            * 常に数値比較して小さい方を代入すれば良い．
          * `amount`の最大値が変わった時に，`INT_MAX`の部分も必要に応じて変える必要がある．
          * 最後の`return`で条件分岐（あるいは三項演算子）が必要．
        * 一応`std::optional<int>`もあるが，実質`-1`と同じだし`int`型と演算できないので今回は略．
    * 以上を考え直した結果，三項演算子を使うなら最後のreturnのとこで使った方がいいかも．ということで[Code4](#Code4)も書いてみた．
      

### Code1
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        int not_achievable = 10001;
        vector<int> num_necessary_coins(amount + 1, not_achievable);
        num_necessary_coins[0] = 0;
        for (int i = 0; i <= amount; ++i) {
            if (num_necessary_coins[i] == not_achievable) continue;

            for (int coin : coins) {
                if (coin <= amount - i) {
                    num_necessary_coins[i + coin] = std::min(num_necessary_coins[i] + 1, num_necessary_coins[i + coin]);
                }
            }
        }

        if (num_necessary_coins[amount] == not_achievable) return -1;
        else return num_necessary_coins[amount];
    }
};
```
### Code2
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        std::sort(coins.begin(), coins.end());
        int num_coins = 0;
        for (int i = coins.size() - 1; i >= 0; --i) {
            while (coins[i] <= amount) {
                amount -= coins[i];
                ++num_coins;
            }
        }
        if (amount == 0) {
            return num_coins;
        }
        return -1;
    }
};
```

### Code3
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> min_num_coins(amount + 1, -1); 
        // i円を構成するのに必要なコインの枚数の最小値がmin_num_coins[i]
        // min_num_coins[i] == -1 --> i円は構成不可能
        min_num_coins[0] = 0;
        for (long long i = 0; i < min_num_coins.size(); ++i) {
            if (min_num_coins[i] == -1) {
                // i円は構成不可能
                continue;
            }
            for (int coin : coins) {
                if (i + coin <= amount) {
                    min_num_coins[i + coin] = (min_num_coins[i + coin] == -1) ?
                                            min_num_coins[i] + 1 :
                                            std::min(min_num_coins[i] + 1, min_num_coins[i + coin]);
                }
            }
        }

        return min_num_coins[amount];
    }
};
```

### Code4
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> min_num_coins(amount + 1, INT_MAX); 
        // i円を構成するのに必要なコインの枚数の最小値がmin_num_coins[i]
        // min_num_coins[i] == INT_MAX --> i円は構成不可能と解釈
        min_num_coins[0] = 0;
        for (long long i = 0; i < min_num_coins.size(); ++i) {
            if (min_num_coins[i] == INT_MAX) {
                // i円は構成不可能
                continue;
            }
            for (int coin : coins) {
                if (i + coin <= amount) {
                    min_num_coins[i + coin] = std::min(min_num_coins[i + coin], min_num_coins[i] + 1);
                }
            }
        }

        return (min_num_coins[amount] == INT_MAX) ? -1 : min_num_coins[amount];
    }
};
```

# Step 2

# Step 1
* いつだかに解いたものが[Code1](#Code1)．
* 時間が経ったのでとりあえず解き直してみる．所要時間17分．
  * とりあえず書いてみた（[Code2](#Code2)）が，頭の中でテストケースを走らせるとおかしいことに気づく．
    * `num_paint_ways[2]`がおかしくなる
      * そもそも2つ目のフェンスは3つ目以降と違って，2つ前のフェンスの色が存在しないので`// 直前のフェンスと同じ色を塗る塗り方 = 2つ前のフェンスと違う色を塗る塗り方．`の部分が`* (k - 1)`でなく`* k`になり，漸化式自体異なってしまう．
    * なので初項として`n == 1`と`n == 2`のケースは自分で設定する必要がある
  * これを修正して[Code3](#Code3)
    * ほぼ[Code1](#Code1)と同じ．違いは以下の通り：
      * 変数名．このくらいのコードならCode1のようにシンプルな変数名でいいかも．
      * 例外処理．Code1は問題文の設定を前提に書いてあるが，Code3はより例外を広く考えている．
      * 計算処理．「1つ前と異なる色」，「2つ前と異なる色で1つ前と同じ色」を分けて足しているかどうか．
  * ちょっと時間がかかりすぎな気がする．
    * 例外ケースの設定に悩んだ．
    * 「動的計画法は与えられた配列の長さより一つ長い配列を用意すると例外ケースを減らせることが多い」という経験則に従って書き始めたが，これが裏目に出た．
      * 今回はそもそも漸化式の形が変わってしまうので，例外ケースは減らせなかった．

### Code1
```cpp
class Solution {
public:
    int numWays(int n, int k) {
        if (n == 1) {
            return k;
        }
        vector<int> ways(n, 0);
        ways[0] = k;
        ways[1] = k*k;

        for (int i = 2; i < n; ++i) {
            ways[i] = ways[i - 1] * (k - 1) + ways[i - 2] * (k - 1);
        }
        return ways[n - 1];
    }
};
```

### Code2
```cpp
class Solution {
public:
    int numWays(int n, int k) {
        if (n <= 0 || k <= 0) {
            return 0;
        }

        vector<int> num_paint_ways(n + 1, 0);
        num_paint_ways[1] = k;
        for (int i = 2; i < num_paint_ways.size(); ++i) {
            // 直前のフェンスと異なる色を塗る塗り方
            num_paint_ways[i] += num_paint_ways[i - 1] * (k - 1); 
            // 直前のフェンスと同じ色を塗る塗り方 = 2つ前のフェンスと違う色を塗る塗り方．
            num_paint_ways[i] += num_paint_ways[i - 2] * (k - 1);
        }
        return num_paint_ways[n];
    }
};
```

### Code3
```cpp
class Solution {
public:
    int numWays(int n, int k) {
        if (n <= 0 || k <= 0) {
            return 0;
        }
        if (n == 1) {
            return k;
        }

        vector<int> num_paint_ways(n, 0);
        num_paint_ways[0] = k;
        num_paint_ways[1] = k * k;
        for (int i = 2; i < num_paint_ways.size(); ++i) {
            // 直前のフェンスと異なる色を塗る塗り方
            num_paint_ways[i] += num_paint_ways[i - 1] * (k - 1); 
            // 直前のフェンスと同じ色を塗る塗り方 = 2つ前のフェンスと違う色を塗る塗り方．
            num_paint_ways[i] += num_paint_ways[i - 2] * (k - 1);
        }
        return num_paint_ways[n - 1];
    }
};
```

# Step 2
* コメント集を見ていたら[フィボナッチ数列](https://discord.com/channels/1084280443945353267/1233603535862628432/1270597315681456219)という単語が出てきた．なぜ？と思ったら再帰で書いた時の計算量の話だった．文脈を整理すると以下の通り：
  * `n`を`n - 1`と`n - 2`から計算するとき，メモ化などせず再帰呼び出しすると計算量はおよそフィボナッチ数列の第n項になる．
  * フィボナッチ数列の第n項はおよそ1.6なので，`n = 50`の時，約`10 ^ 10`程度になるので，流石にきついだろう．

$$
  1.6^{50} = \left(\frac{16}{10}\right)^{50} = \frac{2^{4 \times 50}}{10^{50}} = \frac{1024^{20}}{10^{50}} \approx \frac{10^{3 \times 20}}{10^{50}} = 10^{10}
$$


* ということでメモ化再帰（トップダウン）でも書いてみる．[Code4](#Code4)．所要時間6分．
  * メモ化してあるので計算量はO(n)
  * 書いてから気づいたが，`num_paint_ways[n - 2] == -1`を先に処理すると再帰呼び出しのスタック数が最大でも`n / 2`になってややお得．（`[n - 1]`から処理すると最大`n`になる）

### Code4
```cpp
class Solution {
public:
    int numWays(int n, int k) {
        vector<int> num_paint_ways(n, -1);
        return NumWaysHelper(n, k, num_paint_ways);
    }

private:
    int NumWaysHelper(const int n, const int k, vector<int>& num_paint_ways) {
        if (n <= 0) {
            return 0;
        }
        if (n == 1) {
            return k;
        }
        if (n == 2) {
            return k*k;
        }

        if (num_paint_ways[n - 2] == -1) {
            num_paint_ways[n - 2] = NumWaysHelper(n - 2, k, num_paint_ways);
        }
        if (num_paint_ways[n - 1] == -1) {
            num_paint_ways[n - 1] = NumWaysHelper(n - 1, k, num_paint_ways);
        }

        return num_paint_ways[n - 1] * (k - 1) + num_paint_ways[n - 2] * (k - 1);
    }
};
```

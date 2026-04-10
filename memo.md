[50. Pow(x, n)](https://leetcode.com/problems/powx-n/description/?envType=problem-list-v2&envId=n97t3qfj)

# Step 1
まず書いたのが[Code1](#Code1)だが，`n = - 2^31`のコーナーケースを考え漏らしていてオーバーフロー．修正したのが[Code2](#Code2)．整数型を符号反転させるときにはオーバーフローをチェックする癖をつけたい．というか，演算を見るたびにこの辺のチェックは無意識にできるようにしたい．
#### Code1
```cpp
class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) {
            return 1.0;
        }
        if (n < 0) {
            return 1.0 / myPow(x, -n);
        }

        if (n % 2 == 0) {
            double root = myPow(x, n / 2);
            return root * root;
        } else {
            double root = myPow(x, (n - 1) / 2);
            return root * root * x;
        }
    }
};
```

#### Code2
```cpp
class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) {
            return 1.0;
        }
        if (n < 0) {
            if (n == std::numeric_limits<int>::min()) {
                return 1.0 / myPow(x, - (n + 1)) / x;
            }
            return 1.0 / myPow(x, -n);
        }

        if (n % 2 == 0) {
            double root = myPow(x, n / 2);
            return root * root;
        } else {
            double root = myPow(x, (n - 1) / 2);
            return root * root * x;
        }
    }
};
```

過去に解いたものが[PastCode](#PastCode).
#### PastCode
```cpp
class Solution {
public:
    double myPow(double x, long long n) {
        if (n == 0) return 1.0;

        if (n > 0) {
            if (n % 2 == 0) {
                double sqrt = myPow(x, n / 2);
                return sqrt * sqrt;
            } else {
                double sqrt = myPow(x, n / 2);
                return sqrt * sqrt * x;
            }
        } else {
            return 1 / myPow(x, - n);
        }
    }
};
```

# Step 2
拡張性を考えられていなかった．
- `n`が整数でなく実数の場合は？
- `x = 0`の場合はどうするか．
  -  通常，`n > 0`なら0，`n == 0`なら1，`n < 0`なら引数エラーにしたい．
  -  今の実装だと`n > 0`, `n == 0`では理想通り動きはする．
-  `x < 0`で`n`が実数の場合は？


---
bit演算と相性が良さそう．[cf](https://github.com/TORUS0818/leetcode/pull/47/changes/BASE..204b432db82686a26601908b0c5491cbe6a8ccc6#diff-91647df59bb2863e62120bcb064c143cab9cb1c4e78ed9feab30fc009844b5d0R116)．
可読性はどうだろうか．bit演算ができるときはそうした方が実行速度が多少速くなりそうだが，まあボトルネックではないだろう．そもそもコンパイラが最適化してくれそう．練習がてら書いたのが[Code3](#Code3)．まあ別に読みやすくはなってないな．
#### Code3
```cpp
class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) {
            return 1.0;
        }
        if (n < 0) {
            if (n == std::numeric_limits<int>::min()) {
                return 1.0 / myPow(x, ~n) / x;
            }
            return 1.0 / myPow(x, ~n + 1);
        }

        if (n & 1) {
            double root = myPow(x, n >> 1);
            return root * root * x;
        } else {
            double root = myPow(x, n >> 1);
            return root * root;
        }
    }
};
```

---
ループに書き直す．`n < 0`の時は`n = - 2^31`の時の扱いが面倒なので再帰呼び出しで処理することにする．一旦書いてみたところTLEになったが，原因がぱっと見わからなかったので，時間と興味のある方は考えてみてほしい（[Code4](#Code4)）．原因はコードのすぐ下にあります．
#### Code4
```cpp
class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) {
            return 1;
        }
        if (x == 0) {
            return 0;
        }
        if (n < 0) {
            if (n == std::numeric_limits<int>::min()) {
                return myPow(1 / x, - (n + 1)) / x;
            }
            return myPow(1 / x, - n);
        }

        double base = x;
        int bit = 1;
        double result = 1;
        while (bit <= n) {
            if (bit & n) {
                result *= base;
            }
            base *= base;
            bit <<= 1;
        }
        return result;
    }
};
```

原因：`bit`が`int`型なので，`bit = 2^30`の次は`bit = 2^31`になるが，これがオーバーフローして`bit = 0`になる．以降，whileループの条件がずっとtrueで無限ループになる．

ということで`bit`の型は`long long`に変えた．

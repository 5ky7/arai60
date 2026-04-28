[779. K-th Symbol in Grammar](https://leetcode.com/problems/k-th-symbol-in-grammar/)
# Step 1
とりあえず解いてみたのが[Code1](#Code1)．

#### Code1
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        int num_leaves = static_cast<int>(std::pow(2, n - 1));
        if (n <= 0 || k <= 0 || k > num_leaves) {
            throw "Invalid Argument(s)";
        }
        return KthGrammerHelper(n - 1, k - 1, 0);
    }

private:
    int KthGrammerHelper(int depth, int index, int origin) {
        // depth: depth of the table(0-indexed)
        // index: position of object number(0-indexed)
        if (depth == 0) {
            return origin;
        }
        int num_leaves = static_cast<int>(std::pow(2, depth));
        int new_origin = index < num_leaves / 2 ? origin : 1 - origin;
        int new_index = index % (num_leaves / 2);
        return KthGrammerHelper(depth - 1, new_index, new_origin);
    }
};
```

---
数学的に考えると面白く解けたことを思い出して考えてみる．

0から始まって01,0110, ... と数字が生まれていくが，これを木構造で考える．つまり以下のような木を考える
```
# n = 2の時

        0
    0       1
  0   1   1   0
```
n段目，つまり下端の数字（木構造でいう葉）について，k番目の数字（`ans`とする）が何か聞かれているわけだが，これはkが葉の中で左半分にあるか，右半分にあるかで考えられる．つまり根である0から見て，左側の部分木に含まれているか，右側の部分木に含まれているか，を考え，その部分木について同じ問題を再帰的に考えることができる．具体的にはn段目の葉の数を`K`として，
- kが葉のうち左半分に含まれていれば，0を根とする木の`n-1`段目の`k`番目の数字が`ans`
- kが葉のうち右半分に含まれていれば，（0ではなく！）1を根とする木の`n-1`段目の`k - K`番目の数字が`ans`

これは$`2^i \; (i = n - 1, n - 2, \ldots, 0)`$を`k`から引けるだけ引いていき，引ける時に根の0，1を反転するということに他ならない．最終的に頂点が根の位置にきた時，その数字こそが`ans`である．
初め根は0なので，偶数回引けたら`ans = 0`，奇数回引けたら`ans = 1`になるということだ．そして「$`2^i \; (i = n - 1, n - 2, \ldots, 0)`$を`k`から引けるだけ引く」というのは，`k`の2進展開を計算していることと同義なので，2進展開時の1の数を数えれば良い．ただし，正しくは上の議論を`k`ではなく`k - 1`について行う必要があることに注意する．[Code2](#Code2)．
#### Code2
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        std::bitset<29> k_bit(k - 1);
        if (k_bit.count() % 2 == 0) {
            return 0;
        } else {
            return 1;
        }
    }
};
```

---
かなり以前に一度解いた時のものが[PastCode](#PastCode)．

#### PastCode
```cpp
class Solution {
public:
    int kthGrammar(int n, int k) {
        int column = k;
        map<int, int> row_to_column;
        for (int i = n; i > 0; --i) {
            row_to_column[i] = column;
            column = (column + 1) / 2;
        }

        int digit = 0;
        for (int i = 1; i <= n; ++i) {
            digit = (row_to_column[i] % 2 == 0) ? 1 - digit : digit;
        }
        return digit;
    }
};
```
```cpp
// 数学的考察...kから何回leaf_num / 2を引き算するか？-->kの二進展開に基づく．
class Solution {
public:
    int kthGrammar(int n, int k) {
        bitset<29> k_binary(k - 1);
        if (k_binary.count() % 2 == 0) {
            return 0;
        } else {
            return 1;
        }
    }
};
```
```cpp
// 再帰的二分木探索
class Solution {
private:
    int Helper(int n, int k, int root_val) {
        if (n == 1) {
            return root_val;
        }

        int num_leaf = std::pow(2, n - 1);
        if (k > num_leaf / 2) {
            return Helper(n - 1, k - num_leaf / 2, 1 - root_val);
        } else {
            return Helper(n - 1, k, root_val);
        }
    }
public:
    int kthGrammar(int n, int k) {
        return Helper(n, k, 0);
    }
};
```
```cpp
// simple recursion (comp:O(n), spacial:O(n))
class Solution {
public:
    int kthGrammar(int n, int k) {
        if (n == 1) {
            return 0;
        }

        if (k % 2 == 0) {
            return 1 - kthGrammar(n - 1, k / 2);
        } else {
            return kthGrammar(n - 1, (k + 1) / 2);
        }
    }
};
```
```cpp
// backtrack (comp:O(n), spacial:O(n))
class Solution {
public:
    int kthGrammar(int n, int k) {
        vector<int> index_root_to_leaf(n);
        int column = k;
        for (int i = n - 1; 0 <= i; --i) {
            index_root_to_leaf[i] = column;
            column = (column + 1) / 2;
        }

        int num_leaf = 0;
        for (int i = 0; i < n; ++i) {
            if (index_root_to_leaf[i] % 2 == 0) num_leaf = 1 - num_leaf; 
        }
        return num_leaf;
    }
};
```

# Step 2
立っているビットを数えるには，bitsetのメンバ関数である[std::bitset::count()](https://cpprefjp.github.io/reference/bitset/bitset/count.html)と，符号なし整数型に使える関数である[std::popcount()](https://cpprefjp.github.io/reference/bit/popcount.html)の2つがある．どちらもハードウェア上で最速な方法が実現されるとのこと．

[アセンブラを見てみる](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TEAA2LXJ3ABkCJmwAOT8AI2xSEAAOcgAHdCViFyYvX38gkIys5yEIqNi2BKTUh2wnHJEiFlIiPL8A4PtsR1KmJpaicpj4xJT7Ztb2gq6lSaHIkaqx5IBKe3QfUlROLksAZkjUXxwAajN9jznCYQA6JAvcMy0AQUiiU6ZR0gB5GhFsEQrMQlBAfO99hYAPofAirZ4vMwAdhsr1O6PRAHpMacAFIsABuLBApx8KlO73Ip2MmFJ5MeDP2uApTDm2BYtPQNHOTMet1OP1IdOwpwAksJsMBErc4sQvD5hBB4WiMadsacPKcyBrbLYSWSReDhJCYQjVRSeQARC0AWlOUAIPKeTNOPFW5wsgVOWnUAFZ/QH/fD9lZzRj1RzaWkWARSEotdzZUQlGbVY6LtaIOnPd71Pt8wX8%2B7bPas07HqcLMWcz7C4Xg6G1TiAI4%2BDkplUY9P7TPZ0NlxnM6Sraten1aABik%2BnE4ujbD6uApE2aXjXNOyVTXdOACoM7mtDxD8eeHOw%2BeL%2BGcUgyAQAF7yCSnJR%2BBOnOIATyI2A7L3NpEBLYmHRbtnWZCxpDPS9zXVACiCA05dDJD4iCQFgUPQNJ3y/EUIBYGhvyFIhSAVcRnBMU4SFOSEbSTU5sAJZhTgAdyQJj3gpeNmIIHAiLQ4CjSIE0iGVREkUtBFJNeA4CBoHBuShKEAHFogUDxFIRDjBHQLNhFOdRixRLdTjghDFLicF6HIqEMjSDAFSICADLnSTxMkix9ncWSpMRDzZNOKFUDSXwlGCslyx7StDwATi0JEwncw4mGOHwzguDwkwZV4OLiFowVZAhgCiWkDPOIzOxMwDSGA64QBAWz7MVZyQ1ciTpI8ryaC4dZ6G4X1%2BACLgdHIdBuA8XVrGfTZthFA4%2BHIIhtG69YAGsQF9EJeq4aR%2BDYdaQkG4bRq4fglBAEJFqG7ryDgWAUAwNg0gYRJKGoB6nsYJJgB4H7yBwAkCB2AA1AhsGYn40mYbh5roKzEjOiA4iW8hZVYUgP2h/hUZaD8fjiXR6ku%2BaHo4YQfiYegMauv7sAs4APAkegzt4fgcDYYxgEkanCAAhpGOZ4bsHUeofG/THKAlLbhvoAg4lIHGvBwZHiIIPaWfWGgjGAJQQbBiGoZZuRhDECROBkI3FBUDRkf0fZDA5tAJrsGW4jOyB1kwvpmZtH59lOG1/HQG0lBUaRbgsfh0EY0hSB4n94HWOoGlcCB3GmAIeFCJhMGGSpqgMYpsiEdOC8yIumFz74DCTvoBimbwOmrnpCcaeZK%2BWJJM7mQYS67tvFjzsY3Q2LYdikHq%2BoG5Hjv05JAhtQJpFOYBUFQV0eFuHh7XwYhtTm1Z%2BEunQR3INiOTGJVDG4HbyD2jbyEOyPuFO86FqWk%2B1vvrb9in6njsP9%2BN1EC3WQGgdAj1noUCoBAd6kCQAtDYIvb6Wh9ghH%2BoDbAutwaQ0GjDBghEEZI2ptjdG4sSG43xoTcWJNmBEHJpTZGOA6YM3oEzcWbMOZc2GjzFu/NkZCxFmLQ27wejIxdvLdGitdjDRVmrPgGstY61Btgg2MN5Am0kObQQwhlBqE0NTfQmcjAmEdtYWwhhZZu0vp7HI3tfaR2jrHHAVjE7N2TgEVO2de5ZxzgPKumdC59G8YEnI7d85dzcbXeY3ia6t0GGEoeEwe4NwKH3eJfiO7jxHjNLJW1%2BoP2ntwU4LwABKABZRey9V6ui0LcP2EAd4kCFPvABV0P77SvttXaIBpD7FuEiAZkJooWGikiLQWhAj7GioEApf9n72Ffkfa6ICID3XAR9F60DYGfTQMY5B4yaYA2Bso/WuD%2BCwwIdQIhw1yFkMiDjPGBMnDUPAaTOhFMqbcNpj4emjNmbzQ4SYLhrNYy8PjtTARqBRa7HmiIqW/BxEKywNIw%2Bsc5HXU1iwbWWDTni20aIcQmjZD4t0dbAxIALD2xMeYMx1gLGuwTiNNIXtuA%2Bz9gHFoqAHg9iiGhRgbB/ZsHglEDMSgVofiMCtbADjEhOPju7bovQchuC8SkjOPiEmd3SGXIJarS4lFCRk8JiqW5CDrm0PVESlVmv7hUfxST675HVd3VomqslKGmmPYeeTf5HSKeoOeC8l7HA5jU24tStDb0IE0j0%2Bw3StOPhPLpt91qBHDhYT0YyxkjOivsQImdH4jXmWdC679VodO/r6p%2BJ035tKTRHWZfqa1LJPtHLIrhpBAA%3D%3D)と，確かに
- Intel x86では'popcnt'という命令がすでに用意されている．
- ARM64ではSIMD命令を用いて数えて足すのを並列化している

こんな便利なサイトがあるなんて知らなかった．大学のCPU周りの授業の時に知っていればコンパイルやアセンブリももっと身近に感じられただろうなぁ．

---
分割統治法によるカウントのコード例([cf](https://github.com/dxxsxsxkx/leetcode/pull/46/changes#r2939603367):
```cpp
#include <cstdint>

int popcount(uint32_t x) {
    constexpr uint32_t M1 = 0x55555555u; // 0101...
    constexpr uint32_t M2 = 0x33333333u; // 0011...
    constexpr uint32_t M4 = 0x0F0F0F0Fu; // 00001111...
    constexpr uint32_t M8 = 0x00FF00FFu;
    constexpr uint32_t M16 = 0x0000FFFFu;

    x = (x & M1) + ((x >> 1) & M1);   // 2-bit sums
    x = (x & M2) + ((x >> 2) & M2);   // 4-bit sums
    x = (x & M4) + ((x >> 4) & M4);   // 8-bit sums
    x = (x & M8) + ((x >> 8) & M8);   // 16-bit sums
    x = (x & M16) + ((x >> 16) & M16); // 32-bit sums

    return x;
}
```

何をやっているかと思えば
- 入力をまず2bitsずつに区切って各区画の中で立っているビットを数える．
    - これはつまり，2bitsの各桁を足せば良い．各桁は1か0で，これを足せば0,1,2のどれかになる．そして足した結果は2bitsの範囲に収まるので，各区画を個数(00,01,10のいずれか)に書き換える．
- 次に4bitsずつに区切る．
    - この時上の議論から4bitsの各領域は2bitsずつに分割できて，各2bits領域には入力のその領域に対応する部分の立っているビットの数が入っている．
    - これを足し算したいので，左側を2bitsをビットシフトして足し算する．
- 次に8bitsずつに区切る
    - この時上の議論から8bitsの各領域は4bitsずつに分割できて，各4bits領域には入力のその領域に対応する部分の立っているビットの数が入っている．
    - これを足し算したいので，左側を4bitsをビットシフトして足し算する．
- これを繰り返すと，最終的に入力全体が一つの区画となり，その中の個数が出てくる．

言葉で説明するのが難しいのでこのディレクトリにある画像を見てみると良いかもしれない（多分，分かりにくいが）．なかなか面白い．

仮に各区画の書き換えを並列化して1回の動作でできるなら，上のコードの計算量は$`O\left( \log \left( \text{入力のbit長} \right) \right)`$になる．普通に舐めると$`O\left(\text{入力のbit長}\right)`$なので短縮できたということ．こういう工夫が必要だった時代・分野もあったらしい．今は先人のおかげでコンパイラの最適化が効いたりする．話が逸れるが，面白さって手段の目的化と密接だ．

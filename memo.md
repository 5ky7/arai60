[1011. Capacity To Ship Packages Within D Days](https://leetcode.com/problems/capacity-to-ship-packages-within-d-days)

# Step 1

愚直に解いたものが[Code1](#Code1)で，TLE．方針は
- `weights`の最小値よりcapacityは大きくなくてはならないので，`capacity`をここからスタート
- `capacity`に対して輸送にかかる日数を算出し，これが`days`より大きくなるまで`capacity`をインクリメントしていく．

最悪のケースで`weights = [500] * 5 * 10^4, days = 1`に対して
- `shipWithinDays()`内のwhileループが`2.5 * 10^7`回
- 各ループ中で，`CalcNecessaryDays()`が1回呼び出されるごとにさらにwhileループが`len(weights) = 5 * 10^4`回．

これで合計`10^12`ほどのステップ数．C++でも1000秒くらいはかかりそうなのでTLEになりそう（実際，TLEだった）．

なお計算量は`n = len(weights)`, `M = sum(weights)`に対して
- 時間: $`O(nM)`$
- 空間: $`O(1)`$

#### Code1
```cpp
class Solution {
public:
    int shipWithinDays(const vector<int>& weights, int days) {
        int capacity = *std::max_element(weights.begin(), weights.end());
        while (true) {
            int necessary_days = CalcNecessaryDays(weights, capacity);
            if (necessary_days <= days) {
                break;
            }
            ++capacity;
        }
        return capacity;
    }

private:
    int CalcNecessaryDays(const vector<int>& weights, int capacity) {
        int days = 0;
        auto it_weight = weights.begin();
        while (it_weight < weights.end()) {
            ++days;
            int loading_weight = 0;
            while (it_weight < weights.end() && loading_weight + *it_weight <= capacity) {
                loading_weight += *it_weight;
                ++it_weight;
            }
        }
        return days;
    }
};
```

---
高速化を考える，やりたいことは「適切な`capacity`の値を見つける」．
- 最小でも一つは荷物を運べなきゃいけないから，下限として`min(weights)`
- 最大でも1日で運べれば十分だから，上限として`sum(weights)`

適切な`capacity`はこの間のどこかにある -> 二分探索．ということで書いたのが[Code2](#Code2)．

計算時間は最悪でも
$`
\frac{5 \times 10^4 \times \log (2.5 \times 10^7) \mathrm{steps}}{10^9 \mathrm{steps/s}} \approx 1 \mathrm{ms}
`$程度．

計算量は`n = len(weights)`, `M = sum(weights)`に対して
- 時間: $`O(n \log M)`$
- 空間: $`O(1)`$

#### Code2
```cpp
class Solution {
public:
    int shipWithinDays(const vector<int>& weights, int days) {
        int min_capacity = *std::max_element(weights.begin(), weights.end());
        int max_capacity = std::reduce(weights.begin(), weights.end());
        while (min_capacity <= max_capacity) {
            int capacity = min_capacity + (max_capacity - min_capacity) / 2;
            int necessary_days = CalcNecessaryDays(weights, capacity);
            if (necessary_days > days) {
                min_capacity = capacity + 1;
            } else {
                max_capacity = capacity - 1;
            }
        }
        // capacity < min_capacityなるcapacityに対し，necessary_days > daysである
        // max_capacity < capacityなるcapacityに対し，necessary_days <= daysである
        return min_capacity;
    }

private:
    int CalcNecessaryDays(const vector<int>& weights, int capacity) {
        int days = 0;
        auto it_weight = weights.begin();
        while (it_weight < weights.end()) {
            ++days;
            int loading_weight = 0;
            while (it_weight < weights.end() && loading_weight + *it_weight <= capacity) {
                loading_weight += *it_weight;
                ++it_weight;
            }
        }
        return days;
    }
};
```

---
過去に解いたものが[PastCode](#PastCode)．

#### PastCode
```cpp
class Solution {
private:
    int NecessaryDays(vector<int>& weights, int capacity) {
        // computational: O(weights.size())
        int necessary_days = 1;
        int load = 0;
        for (int i = 0; i < weights.size(); ++i) {
            if (load + weights[i] > capacity) {
                load = 0;
                ++necessary_days;
            }
            load += weights[i];
        }
        return necessary_days;
    }
public:
    int shipWithinDays(vector<int>& weights, int days) {
        int min_search = *max_element(weights.begin(), weights.end()); // 3
        int max_search = accumulate(weights.begin(), weights.end(), 0) + 1; // 4
        int capacity = (min_search + max_search) / 2; // 3
        while (min_search < max_search) {
            if (NecessaryDays(weights, capacity) > days) min_search = capacity + 1;
            else max_search = capacity;
            capacity = (min_search + max_search) / 2;
        }

        return capacity;
    }
};
```

```cpp
// greedy computational: O(n*m), spacial: O(1) (n = weights.size(), m = sum(weights))
class Solution {
private:
    int NecessaryDays(vector<int>& weights, int capacity) {
        int count = 1;
        int loading = 0;
        for (int i = 0; i < weights.size(); ++i) {
            if (loading + weights[i] > capacity) {
                loading = 0;
                ++count;
            }
            loading += weights[i];
        }
        return count;
    }
public:
    int shipWithinDays(vector<int>& weights, int days) {
        int capacity = *std::max_element(weights.begin(), weights.end());

        while (NecessaryDays(weights, capacity) > days) {
            ++capacity;
        }

        return capacity;
    }
};
```

# Step 2
「必要日数を求める -> これが`days`以内か判定する」のロジックを関数に置く（[cf](https://github.com/fhiyo/leetcode/pull/45/changes#diff-3e42d068b82e2a1be434dc989edc077d304c433f9a25ad4a2b3bc8f9223e43bcR51)）．[Code3](#Code3)．

こっちの方が確かにロジックとしてクリアである．

#### Code3
```cpp
class Solution {
public:
    int shipWithinDays(const vector<int>& weights, int days) {
        if (weights.empty()) {
            return 0;
        }
        if (days <= 0) {
            return 0;
        }

        int min_capacity = *std::max_element(weights.begin(), weights.end());
        int max_capacity = std::reduce(weights.begin(), weights.end());
        while (min_capacity <= max_capacity) {
            int capacity = min_capacity + (max_capacity - min_capacity) / 2;
            if (!CanBeShipped(weights, capacity, days)) {
                min_capacity = capacity + 1;
            } else {
                max_capacity = capacity - 1;
            }
        }
        // capacity < min_capacityなるcapacityに対し，necessary_days > daysである
        // max_capacity < capacityなるcapacityに対し，necessary_days <= daysである
        return min_capacity;
    }

private:
    bool CanBeShipped(const vector<int>& weights, int capacity, int days) {
        return CalcNecessaryDays(weights, capacity) <= days;
    }
    int CalcNecessaryDays(const vector<int>& weights, int capacity) {
        int days = 0;
        auto it_weight = weights.begin();
        while (it_weight < weights.end()) {
            ++days;
            int loading_weight = 0;
            while (it_weight < weights.end() && loading_weight + *it_weight <= capacity) {
                loading_weight += *it_weight;
                ++it_weight;
            }
        }
        return days;
    }
};
```

--- 
`min_capacity`の初期値．せいぜい大きくても500だから，これをわざわざ`std::max_element`で求めるよりも，`min_capacity = 0`で初めて500の誤差は二分探索の方に任せる方が良い．二分探索なら500は$`\log 500 \approx 9`くらいに落ちるので．

**と思ったが**，これはダメだ．日数計算の結果が無限大に発散して面倒なことになる．

原因は`CanBeShipped`のロジック内にある．日数を求める際に暗に`capacity >= max(weights)`を仮定してしまっている．
[こちらのコメント](https://github.com/Ryotaro25/leetcode_first60/pull/51/changes#r1987809727)を読んで気がついた．逆に言えばこの仮定を外してチェックを挟めば良い．[Code4](#Code4)．

#### Code4
```cpp
#include <execution>

class Solution {
public:
    int shipWithinDays(const vector<int>& weights, int days) {
        if (weights.empty()) {
            return 0;
        }
        if (days <= 0) {
            return 0;
        }

        int min_capacity = 0;
        int max_capacity = std::reduce(std::execution::par, weights.begin(), weights.end());
        while (min_capacity <= max_capacity) {
            int capacity = min_capacity + (max_capacity - min_capacity) / 2;
            if (!CanBeShipped(weights, capacity, days)) {
                min_capacity = capacity + 1;
            } else {
                max_capacity = capacity - 1;
            }
        }
        return min_capacity;
    }

private:
    int CanBeShipped(const vector<int>& weights, int capacity, int days) {
        int necessary_days = 1;
        int loading_weight = 0;
        for (int weight : weights) {
            if (weight > capacity) {
                return false;
            }
            if (loading_weight + weight > capacity) {
                loading_weight = 0;
                ++necessary_days;
            }
            loading_weight += weight;
        }
        return necessary_days <= days;
    }
};

```

---
`std::max_element()`の他に`std::ranges::max`もある．こちらは
- 返り値がイテレータでなく値
- 引数もイテレータではなく変数そのもの
- 線形に舐めるだけでなく，実行ポリシーを渡すと並列化できる(C++26)

という点で違いがある．なおLeetCodeではC++23のため並列化はできなかった．

---
pythonみたいに`range()`が使えれば`lower_bound`をそのまま使えるのになー，と思っていたら[`std::iota`](https://cpprefjp.github.io/reference/numeric/iota.html)とかいう便利なものを見つけた．これと`lower_bound`を使って書いたのが[Code5](#Code5)．

しかしTLE．冷静に考えれば，`std::vector<int> capacities(max_capacity - min_capacity + 1);`の時点で計算時間が$`O(\mathrm{sum(weights)})`$になっているわけだ．

ではなぜ[このpythonコード](https://github.com/mamo3gr/arai60/blob/1011_capacity-to-ship-packages-within-d-days/1011_capacity-to-ship-packages-within-d-days/step2.py)が通るのか？と思って調べたら，pythonの`range(n)`はO(n)ではなくO(1)らしい．知らなかった．
#### Code5
```cpp
#include <execution>

class Solution {
public:
    int shipWithinDays(const vector<int>& weights, int days) {
        if (weights.empty()) {
            return 0;
        }
        if (days <= 0) {
            return 0;
        }

        int min_capacity = 0;
        int max_capacity = std::reduce(std::execution::par, weights.begin(), weights.end());

        std::vector<int> capacities(max_capacity - min_capacity + 1);
        std::iota(capacities.begin(), capacities.end(), min_capacity);
        std::transform(
            capacities.begin(),
            capacities.end(),
            capacities.begin(),
            [&](int capacity) {
                return CanBeShipped(weights, capacity, days);
            }
        );
        auto it = std::lower_bound(
            capacities.begin(),
            capacities.end(),
            true
        );
        int pos_it = std::distance(capacities.begin(), it);
        return pos_it;
    }

private:
    bool CanBeShipped(const vector<int>& weights, int capacity, int days) {
        int necessary_days = 1;
        int loading_weight = 0;
        for (int weight : weights) {
            if (weight > capacity) {
                return false;
            }
            if (loading_weight + weight > capacity) {
                loading_weight = 0;
                ++necessary_days;
            }
            loading_weight += weight;
        }
        return necessary_days <= days;
    }
};
```

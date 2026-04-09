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

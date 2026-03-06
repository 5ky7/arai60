[122. Best Time to Buy and Sell Stock II](https://leetcode.com/problems/word-break/description/?envType=problem-list-v2&envId=n97t3qfj)

# Step 1
初めて解いてからだいぶ時間が経ったので解き直す（[Code1](#Code1)）．
- 今持っていない時に次の時刻で上がるなら買えば良い（∵少なくとも買えば次の時刻にうることで利益を生み出せるが，買わないと利益は生み出せない）
- 持ってる時に次の時刻で下がるなら買えば良い（∵少なくとも今売れば次の時刻に売るより利益が生み出せる．もしそのあともっと上がるなら，今より低い次の時刻で買ってから上がった後に売れば良い）

---
これって冷静に考えると全ての価格が上がる直前で買って上がった直後に売る（価格が上がった時だけ積分する）ことだから，以下の[Code2](#Code2)のように書き直せる．

このコードにして気づいたが，Code1もCode2も`if (prices.size() <= 1)`の例外処理はなくても動く．が，入れた方がわかりやすいし読んだ人が「本当にいらないの？」と考える時間も無駄なので入れておく．


#### Code1
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <= 1) {
            return 0;
        }

        bool has_stock = false;
        int buy_price;
        int total_profit = 0;
        for (int i = 0; i < prices.size() - 1; ++i) {
            if (has_stock) {
                if (prices[i] > prices[i + 1]) {
                    total_profit += prices[i] - buy_price;
                    has_stock = false;  
                }
                continue;
            }
            if (!has_stock) {
                if (prices[i] < prices[i + 1]) {
                    buy_price = prices[i];
                    has_stock = true;
                }
                continue;
            }
        }
        // 最後の時刻に持っていたら売る
        if (has_stock) {
            total_profit += prices.back() - buy_price;
        }
        return total_profit;
    }
};

// テストケース
// []
// [1]
// [1,3]
// [3,1]
// [1,2,3]
// [3,2,1]
// [1,3,2]
// [2,3,1]
// [2,1,3]
// [3,1,2]
// [1,4,2,5]
// [5,2,4,1]
// [7,1,5,3,6,4]
```

#### Code2
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <= 1) {
            return 0;
        }

        int total_profit = 0;
        for (int i = 0; i < prices.size() - 1; ++i) {
            if (prices[i] < prices[i + 1]) {
                total_profit += prices[i + 1] - prices[i];
            }
        }
        return total_profit;
    }
};
```

いつだか解いた時のが以下の[PastCode](#PastCode)から続く3つのコード．
#### PastCode
```cpp
// Brute Force with Dynamic programing
// Time Limit Exceeded
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        vector<int> memo(prices.size(), -1);
        return maxProfitHelper(prices, dp, 0);
    }

private:
    int maxProfitHelper(vector<int>& prices, vector<int>& memo, int start) {
        if (start >= prices.size()) return 0;
        if (memo[start] != -1) return memo[start];

        int max = 0;
        for (int i = start; i < prices.size(); ++i) {
            for (int j = i + 1; j < prices.size(); ++j) {
                max = std::max(max, maxProfitHelper(prices, memo, j + 1) + prices[j] - prices[i]);
            }
        }

        memo[start] = max;

        return max;
    }
};
```

```cpp
// 谷to頂点を全て掻っ攫う
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int valley = prices[0];
        int peak = prices[0];
        int profit = 0;
        for (int i = 0; i < prices.size(); ++i) {
            while (i < prices.size() - 1 && prices[i] >= prices[i + 1]) {
                ++i;
            }
            valley = prices[i];
            while (i < prices.size() - 1 && prices[i] <= prices[i + 1]) {
                ++i;
            }
            peak = prices[i];
            profit += peak - valley;
        }
        return profit;
    }
};
```

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int profit = 0;
        for (int i = 0; i < prices.size() - 1; ++i) {
            if (prices[i] < prices[i + 1]) {
                profit += prices[i + 1] - prices[i];
            }
        }
        return profit;
    }
};
```




# Step 2

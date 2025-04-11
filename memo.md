# Step 1
- C++のクラスを復習（構造体しかやったことがなかったため）
  - C++においてクラスと構造体の違いは，デフォルトのアクセシビリティ
    - structは`public`
    - classは`private`
    - C++ではstructでもメンバ関数が定義できるので，違いとして「関数が定義できるかどうか」は誤り
- 取得するのは最小の値のみなので，k個の要素を持つプライオリティキューで実装すれば良さそう．
  - リファレンスでプライオリティキューを検索するとC++には`priority_que<>`というコンテナがあるそうなので，ありがたく使わせていただく．
  - ただしデフォルトでは降順で整理されるので，入力値のテストスコアを-1倍したものをキューに突っ込んでいく
    - もしかしたらこれは良くないかも．他人が読むことを考えたらちゃんと昇順に整理すべき？（リファレンスにそのコード例もあるし）
  - で，実装した結果（次のコード），エラー
```c++
class KthLargest {
public:
    KthLargest(int k, vector<int>& nums) {
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        for (int i=0; i<k; i++) {
            score_que.push(-nums.at(i));
        }
    }
    
    int add(int val) {
        if (-score_que.top() < val) {
            score_que.pop();
            score_que.push(-val);
        }

        return -score_que.top();
    }
private:
    std::priority_queue<int> score_que;
};
```
- 問題文の条件を見ると`1 <= k <= nums.length + 1`とかある．気持ち悪い．`k <= nums.length`（ちょうど都合いい）にするか`k <= 10^4`（一般）にすればいいのに．まあ例外処理するか...
  - `k > nums.size()`の時`k = k-1`とする処理を入れたがエラー
    - これはメンバ関数`add()`で`score_que.top()`を呼び出してしまっていることに由来．`k=1`の時，`score_que`は空なのに`top()`を呼び出しエラー
  - `add`のとこで場合わけするのは綺麗でない気がした．コンストラクタの`k > nums.size()`時の処理を変更してダミースコアを追加することにした．Accepted.（次のコード）

```c++
class KthLargest {
public:
    KthLargest(int k, vector<int>& nums) {
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        if (k > nums.size()) {
            score_que.push(100000);
            k = k-1;
        }
        for (int i=0; i<k; i++) {
            score_que.push(-nums.at(i));
        }
    }
    
    int add(int val) {
        if (-score_que.top() < val) {
            score_que.pop();
            score_que.push(-val);
        }

        return -score_que.top();
    }
private:
    std::priority_queue<int> score_que;
};
```

# Step 2

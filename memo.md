# Step 1
- C++のクラスを復習（構造体しかやったことがなかったため）
  - C++においてクラスと構造体の違いは，デフォルトのアクセシビリティ
    - structは`public`
    - classは`private`
    - C++ではstructでもメンバ関数が定義できるので，違いとして「関数が定義できるかどうか」は誤り
  - 使い分けについては[Googleのスタイルガイド](https://ttsuki.github.io/styleguide/cppguide.ja.html#Structs_vs._Classes)によると
    - `struct`は"passive objects that carry data"にのみ用い，メンバ変数，メンバ関数は全てpublicでなければならず，従ってユーザーなどからアクセスされうるこれらに不変性などを仮定してはいけない．
    - それ以外のアクセス制限や，今後の拡張があり得る場合は`class`にする．迷ったら`class`を使っとけ．
    - なお`class`と`struct`で命名規則が異なることに注意する．`class`のメンバ変数は後ろにアンダースコアをつける(e.g. `num_components_`)が，`struct`の場合はつけない．
- 取得するのは最小の値のみなので，k個の要素を持つプライオリティキューで実装すれば良さそう．
  - `nums`はk個以上要素を持つ可能性があるので，ソートして降順にしておく．
  - リファレンスでプライオリティキューを検索するとC++には`priority_que<>`というコンテナがあるそうなので，ありがたく使わせていただく．
  - ただしデフォルトでは降順で整理されるので，入力値のテストスコアを-1倍したものをキューに突っ込んでいく
    - もしかしたらこれは良くないかも．他人が読むことを考えたらちゃんと昇順のプライオリティキューとして宣言すべき？（リファレンスにそのコード例もあるし）
  - で，実装した結果（次のコード），エラー
```c++
class KthLargest {
public:
    KthLargest(int k, vector<int>& nums) {
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        for (int i = 0; i < k; ++i) {
            score_que.push(-nums[i]);
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
  - `add`のとこで場合わけするのは綺麗でない気がした．コンストラクタの`k > nums.size()`時の処理を変更してダミースコア-10001( < min{`nums[i]`})を追加することにした．Accepted.（次のコード）
    - ただエラー処理として適切かは怪しい．実用上は指定された範囲外のスコアはダミーとして扱う処理を入れる必要がある．
    - 問題文の条件が`1 <= k <= nums.length + 1`なので，1度でも`add()`が呼ばれれば弾かれてどうにでもなるが...

```c++
class KthLargest {
public:
    KthLargest(int k, vector<int>& nums) {
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        if (k > nums.size()) {
            score_que_.push(100000);
            k = k-1;
        }
        for (int i = 0; i < k; i++) {
            score_que_.push(-nums[i]);
        }
    }
    
    int add(int val) {
        if (-score_que_.top() < val) {
            score_que_.pop();
            score_que_.push(-val);
        }

        return -score_que_.top();
    }
private:
    std::priority_queue<int> score_que_;
};
```

# Step 2
- プライオリティキューを昇順で宣言したのが以下のコード
  - ついでに`que`というのは宣言を見たらわかるので`score_que_`を`scores_`に変更．
  - 入力の`nums`が勝手にソートされてたら気持ち悪い気もするので（例えばindexを出席番号と対応させる場合），参照渡しからコピー渡しに変更．
  - `k > nums.size()`の時の処理を，`k <= nums.size() + 1`に限らないケースに拡張(`if`を`while`に）
  - `k`が0以下の値の時にエラーを出して弾く機能を追加
```c++
class KthLargest {
public:
    KthLargest(int k, vector<int> nums) {
        if (k <= 0) {
            throw invalid_argument("KthLargest の k は 1 以上でなければなりません");
        }
        sort(nums.begin(), nums.end(), greater<int>());
        while (k > nums.size()) {
            scores_.push(-10001);
            k = k-1;
        }
        for (int i = 0; i < k; ++i) {
            scores_.push(nums[i]);
        }
    }
    
    int add(int val) {
        if (scores_.top() < val) {
            scores_.push(val);
            scores_.pop();
        }
        return scores_.top();
    }
private:
    priority_queue<int, vector<int>, greater<int>> scores_;
};
```
- 思ったけど，`sort()`に`greater<T>()`を渡すと降順なのに`priority_queue<>`に`greater<T>()`を渡すと昇順になるのがややこしい．

- この問題でpriority_queueにいきなりいくのは違和感があるらしい．
  - 平衡2分木としてmapがあり，昇順に並んでいるのだから，とのこと．
    - 言われれば確かに`map`には順序付きで並んでるという記憶がある．が，他の記憶とうまく噛み合わないことに気づいた．
      - 二分木は配列`A`を用意して，`A[i]`の左の子供を`A[2*i + 1]`, 右の子供を`A[2*i + 2]`として格納するものだと思っていた．
      - でも二分探索木って別にルートが最小なわけじゃないよな？最大最小は木の端っこだよな？と思って調べた結果，mapの内部構造の赤黒木は**配列ではなく連結リストで保持**されていることがわかった．
      - ただし`map`は重複なしなので，今回は`multiset`を使う．
    - ということで，リストとして小さいものから大きいものへ並んでいて，リストのノードのポインタが動的に変更される仕組みなので，begin()でkth largestなスコアが取得可能（以下のコード）
```cpp
class KthLargest {
public:
    KthLargest(int k, vector<int> nums) {
        sort(nums.begin(), nums.end(), greater<int>());
        if (k > nums.size()) {
            scores_.insert(-10001);
            k = k-1;
        }
        for (int i = 0; i < k; ++i) {
            scores_.insert(nums[i]);
        }
    }
    
    int add(int val) {
        if (*scores_.begin() < val) {
            scores_.erase(scores_.begin());
            scores_.insert(val);
        }
        return *scores_.begin();
    }
private:
    multiset<int> scores_;
};
```



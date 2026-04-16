# Step 1
- nums内の要素とその登場回数のペアを扱いたい．特に登場回数が変化し，かつ登場回数の大小を扱いたい（ソート，あるいは最大の取得）
  - `vector<int>`で引数をスコアに，値を登場回数に対応させる
      - 空間計算量がとんでもないことになりそうだし，でかい配列になるからソートも時間かかりそうなので却下．
  - `std::map<int, int>`は，`map['hoge']++`で登場回数を変化させられるが，ソートが登場回数ではなく要素（キー）によってなされる
    - プライオリティキューと同様，キーが最小（or最大）の要素をO(1)で取得できる勘違いしていたが，赤黒木で実装されているためそんなことはない．キー最大or最初を辿るためO(log N)必要．
  - `std::priority_que<pair<int, int>>`
    - 最大をO(1)で取得可能，O(log N)で取得&削除可能なので良さげ．ただし一度入れた要素を変更する（登場回数を数えていく）のはめんどくさい．
  - `std::vector<pair<int, int>>` 
    - 新しい要素が来るたびにソートし直せば各ソートがO(Nlog N)でできるからlog Nを定数と見て仕舞えばO(N^2)くらい
- ふと思った点として，「*登場回数が変化*」と述べたとおり動的に保持する前提だったが，703. Kth Largest Element in a Streamと違って新しく追加される機能はないので，静的で良い？
  - 一旦`std::map`に入れていって(O(log N))，それを順次`std::priority_queue`に入れていけば(1要素あたりO(log N))，ロジックがシンプルかつトータル計算量O(N log N)でいい感じ．これを実装したらAccepted（以下のコード）

```c++
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std:map<int, int> score_to_num_appear; // mapの要素は{スコア, 登場回数}
        for (int i : nums) {
            score_to_num_appear[i]++;
        }

        std::priority_queue<std::pair<int, int>> num_appear; // 各要素は登場回数でソートされるように{登場回数, スコア}にする
        for (auto pair : score_to_num_appear) {
            num_appear.push({pair.second, pair.first});
        }

        vector<int> k_most_frequent_elements; // 出力用
        for (int i = 0; i < k; i++) {
            k_most_frequent_elements.push_back(num_appear.top().second);
            num_appear.pop();
        }

        return k_most_frequent_elements;
    }
};
```
 

# Step 2
- `multimap`も一応昇順に整理されているので，`priority_queue`の代わりに使える（以下のコード）．
  - 登場回数は重複の可能性有りなので`map`ではなく`multimap`で．
```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std::map<int, int> score_to_num_appear; // mapの要素は{スコア, 登場回数}
        for (int i : nums) {
            score_to_num_appear[i]++;
        }
        cout << "step1" << endl;

        std::multimap<int, int, greater<int>> num_appear; // 各要素は登場回数でソートされるように{登場回数, スコア}にする
        for (auto pair : score_to_num_appear) {
            num_appear.insert({pair.second, pair.first});
        }
        cout << "step2" << endl;

        vector<int> k_most_frequent_elements; // 出力用
        for (int i = 0; i < k; i++) {
            cout << "step3 i:" << i << endl;
            auto p = *num_appear.begin();
            cout << "step3 i:" << i << endl;
            k_most_frequent_elements.push_back(p.second);
            cout << "step3 i:" << i << endl;
            num_appear.erase(num_appear.begin());
            cout << "step3 i:" << i << endl;
        }

        return k_most_frequent_elements;
    }
};
```
- [Quick Sort](https://discord.com/channels/1084280443945353267/1183683738635346001/1185972070165782688)について．（後で整理する）
  - 平均時間計算量は分割統治法がうまく働く時でO(n log n)．最悪時間計算量は小問題におけるピボットが常に範囲内で最小（あるいは最大）になるときでO(n^2)
  - 空間計算量については，in-placeで実装できるため再帰呼びだしが問題になる．平均空間計算量はO(log n)．最悪空間計算量はO(n)，
    - pivotによる分割後に2回の再起呼び出しがあるが，先にサイズが小さい方の部分配列に対して再起呼び出しを行うと平均的なケースではO(log n)，最悪なケースでは逆にO(1)の空間計算量で済む．
    - この実現には，再起をループにして書き直すか，[末尾再帰最適化](https://zenn.dev/kj455/articles/dfa23c8357b274)を行えば良い．
  - ピボット選択は「常に整列させたい範囲の中の最小値（あるいは最大値）」を取らないようにしたい．そのために以下の選択がある：
    - ランダムに選択する
    - 配列の一部の要素（先頭と末尾と中間の3要素の中央値）
    - [中央値の中央値](https://ja.wikipedia.org/wiki/中央値の中央値)を用いると
  - マージソートとのプロコン
    - 平均計算量は同じ
    - 最悪計算量はマージの方が小さい
    - 安定性はマージのみ
    - 空間計算量はクイックソートのが小さい（マージソートin-placeに実装できず補助配列としてO(n)を要する．）
    - 空間的局所性はInplaceに実装できるクイックソートの方が良い．
    - 並列化はマージの方がやりやすい（クイックソートは先に指定範囲を全て舐める（分割）必要があるが，マージは後で舐める（統合）ことになるので）
  - [Quick Select](https://ja.wikipedia.org/wiki/クイックセレクト)

# Step 3
- pair型をそのまま出力用の配列に代入するミス
- return文の最後の`;`つけ忘れ

***思考過程に「なんか変だな」「考えるべきことがズレているな」「そこよりまずあっちを考えないと」などありましたら，是非指摘していただきたいです！***
# Step 1
## とりあえずnaiveに実装
  - 部分列の和が`k`に等しいかみていく．最悪時間計算量はO(n^3)．空間計算量はO(1)．（以下[1つ目のコード](#Code1)．Time Limit Exceeded）．
  - 要素数ごとに場合分けして考える：
  - Example 2において，要素数1のものは[3], 要素数2のものは[1,2]，要素数3のものはなし，
  - 部分列の取得はspanを使うと可読性と速度の両立が可能（参照であることに注意）
    - コード書いてみたらポインタを用いるので十分可読性があるのでspanのメリット活かしてないのでspanは使わないことに
  - Outputはたかだかnumsから2つの要素を選んでくる場合の数なのでintの範囲に収まる．
  - コードの読みやすさについて，`j`を要素数にして`nums[i+j]`を足すより，`j = i to nums.size()`にして`nums[j]`を足していく方がわかりやすいかな？
## 高速化を考える．
  - 今の実装だとstartからendの和を求めるとき，毎回計算し直しているが，要素数jが増えていくのならjとj+1の計算はほとんど変わらないので無駄がある．
    - そこで`sum`を用意してjが増えるたびに`sum`に追加で足していくようにした（以下[2つ目のコード](#Code2)．Time Limit Exceeded）．最悪時間計算量はO(n^2).空間計算量はO(1)，
    - とけたテストケースの数は増えているので多少速くなった（62 --> 92 out of 93）が，まだ足りない．
  - Hash Mapを用いた高速化ができないか考える
    - [1. Two Sum](https://leetcode.com/problems/two-sum/description/)では，
      - 線形でvectorの全要素をmapに移していく．O(n)．
      - 移すたびにmapの中を検索する．O(log n)．
    - でO(n^2)をO(n log n)に落とせた．
    - 一方で今回同じ方針でやろうとすると，mapに移していくにしてもsubarrayがO(n^2)個あるのでmapに移すだけでO(n^2)必要，この時点でnaive実装と同じ計算量になってしまう．
  - 諦めて答えを見る．
    - 累積和を使えば良いのか．確かに`nums`の累積和`sums`に対して`sums[j] - sums[i]`で部分列の表現ができる．
      - というか2つ目のコードをさらに改善しよう，という発想でも累積和は出てきそう．
      - そういえば累積和（積分画像）はsizeを+1して，index 0にはvalue 0を入れておくと処理の手間が省けた記憶があるので，そうする．
      - と思ったが，冷静に考えて計算量O(n^2)のままじゃないか．と思ってとりあえず実装したら通った（以下3つ目のコード，2000msくらい）．
        - 2つ目が通らなくて3つ目が通る理由がわからない
          - `sum`を介しているぶん，代入操作で時間をとっているのか？
          - キャッシュの利用については，どちらも似たようなアクセスの仕方（内側のループごとに配列の隣の要素にアクセスしていく）なので大差ないはず．
        - o4-mini-highに聞いた結果：
          - `sum`を用いるとjループを回すにはj-1のループが必要になるが，`sum`を用いない後者の方はjのループにj-1のループの結果が必要ないので並列化が可能．（データ依存の有無，コンパイラ最適化の受けやすさ）
          - 3つ目のコードについて，`cumulative_sums[end]`のアクセスにおいてポインタインクリメントでアドレスが求まるので速いらしい
            - だったら2つ目のコードもjの取り方を変えて`nums[i+j]`を`nums[j]`にしたら速くなる？ --> **速くなった！** Acceptされた（2700msくらい）．
            - for文で配列アクセスを回すときはループ変数をindexにすると速くなる事がある．可読性を損なわない限りはそうしよう．
              - ただまだ実行速度に700msくらいの差はあって，leetcodeの実行時間が不正確といえどデータ依存による速度の差はちゃんと反映されているようだ．
      - 大体~10^3msくらいが単純にsubarrayを全て舐めるアルゴリズムの限界？
        - 計算量だけみるとifの条件文でkと等しいかの比較がsubarraysの数，つまりO(n^2)だけ必要になる．定数倍は上記のような工夫で下げられるが，オーダーをこれ以下に抑えるには根本的に別の考え方が必要．
    - Hash Mapを使った高速化（[4つ目のコード](#Code4), accepted）．平均時間計算量O(n)，空間計算量O(n)．
      - [1. Two Sum](https://leetcode.com/problems/two-sum/description/)と同様にhash mapを利用するにはどうすれば良いかを考える．
      - 累積和について[1. Two Sum](https://leetcode.com/problems/two-sum/description/)と同様のことを行えば良い．
      - `++sum_to_count[sum];`は`if`の前には置けない．`k = 0`の時にmapに追加された要素をカウントしておかしいことになる．
      - `solution`の増やし方は，累積和中の`sum`の登場回数と結びついているので，setでは実装できない．
## 前置インクリメントと後置インクリメントについて．
  - 疑問の発端は「後置の方が（私にとっては）可読性が高いが前置の方が速いよな，どうしようか」
  - 基本前置の方が速いと思っていたが，[そうとは限らない](https://cpp.aquariuscode.com/preincriment-vs-postincriment)らしい．
    - 確かにgccでのintインクリメントは後置の方が速いというデータがある（clang, vc++は前置の方が速い）
    - とはいえ，後置の方が良いというソースはゲームエンジンという特殊な環境を前提にしている節がある．
  - [Google Style Guide(日本語版)](https://ttsuki.github.io/styleguide/cppguide.ja.html#Preincrement_and_Predecrement)によれば基本的には前置にするとのこと．
    - どうやら後置の方が読みやすいという人は少数派らしい．
  - いずれにせよここがボトルネックになるか？という議論が必要で，そうでないならとりあえず前置で良いという結論に至った．

### Code1
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
        for (int i = 0; i < nums.size(); ++i) { // iはスタート位置
            for (int j = 1; j <= nums.size() - i; ++j) { // jはsubarrayの要素数
                auto start = nums.begin() + i;
                auto end = nums.begin() + i + j;
                if (std::reduce(start, end) == k) {
                    ++count;
                }
            }
        }
        return count;
    }
};
```
### Code2
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
        for (int i = 0; i < nums.size(); ++i) { // iはスタート位置
            int sum = 0;
            for (int j = 0; j < nums.size() - i; ++j) { // jはsubarrayの要素数
                sum += nums[i+j];
                if (sum == k) ++count;
            }
        }
        return count;
    }
};
```
### Code3
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> cumulative_sums(n + 1);
        cumulative_sums[0] = 0;
        for (int i = 1; i <= n; ++i) {
            cumulative_sums[i] = nums[i-1] + cumulative_sums[i-1];
        }

        int count = 0;
        for (int start = 0; start < n; ++start) {
            for (int end = start + 1; end <= n; ++end) {
                if (cumulative_sums[end] - cumulative_sums[start] == k) {
                    ++count;
                }
            }
        }
        return count;
    }
};
```
### Code4
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        std::unordered_map<int, int> sum_to_count;
        sum_to_count.insert({0,1});
        int sum = 0;
        int solution = 0;
        for (int i = 0; i < nums.size(); ++i) {
            sum += nums[i];
            if (sum_to_count.contains(sum - k)) {
                solution += sum_to_count[sum - k];
            }
            ++sum_to_count[sum];
        }
        return solution;
    }
};
```
# Step 2

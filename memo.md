# Step 1
- 総当たりだと最悪でcandidates.lengh ^ (target/2)くらいのオーダー
- `target - candidates[i]`に対する再帰が使えそうだが，naiveな再帰は総当たりと変わらないので工夫 --> メモ化
- `[2,3,3]と[3,3,2]`を同じものと捉えたいので，一旦順序なし集合（数値の組）の順序なし集合（メモ化，数値の組の組）で考える
- と思ったが，mapは重複を許さないので`[2,3,3]`は`[2,3]`になる．vectorで毎回sortするか順序保って挿入すれば良いか．
- ここちょっと重くなりそうなので`priority_queue`とか挿入がO(log N)で済むやつを使う手も考えたけど，一旦naiveにvectorで実装してみる（以下1つ目のコード，コンパイルエラー）
  - `map<T>::insert()`はメモリを再確保する．戻り値のイテレータを受け取っても再帰でinsertが呼び出されるので結局受け取ったイテレータは，プログラムが再帰呼び出しから帰ってきた時点では意味をなさない．
  - あとHelper関数はprivateに入れるべきですね，おそらく...
- 再帰と相性がいいのはスタックなので，`push_back()`と`pop_back()`でこの辺を処理すれば良い．（以下2つ目のコード，Accepted）
  - しかも`push_back()`と`pop_back()`は定数時間
  - 1つ目のコードは`for`文が非効率．`for`のたびに全ての`candidates`の要素全てを再調査している．例えば`candidates = [1,2,3,4,5]`だとしたら，`3 -> 2 -> 2-> 3 -> 1`みたいな付け加え方を許している．下から順に選ぶようにすれば良い（3を一旦選んだらあとは3,4,5のどれかから選ぶということ）
  - それには，`candidates`の中から一つ選んで付け加える候補に対して，indexの下限をHelper関数の引数に渡してやれば良い（`int start`)
```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        set<vector<int>> memo; 
        vector<int> candidates_selected;

        combinationSumHelper(candidates, target, memo, candidates_selected);
        vector<vector<int>> memo_vec(memo.begin(), memo.end());
        // set の要素をソート済みのまま vector にコピー
        return memo_vec;
    }

    void combinationSumHelper(vector<int>& candidates, int target, set<vector<int>>& memo, vector<int>& candidates_selected) {
        // base case
        if (target < 0) {
            return;
        }
        if (target == 0) {
            // メモに今のcandidates_selectedを追加
            memo.insert(candidates_selected);
            return;
        }

        // recursion step
        for (int candidate : candidates) {
            // 順序を崩さないよう挿入（重複防止）．lower_boundはコンテナが空でも終端イテレータを返してくれる．
            auto it = std::lower_bound(candidates_selected.begin(), candidates_selected.end(), candidate);
            auto inserted_it = candidates_selected.insert(it, candidate); 
            
            // 今のcandidates_selectedがメモに存在しなければcombinationSumHelperでmemoに追加するかチェック．存在するなら何もしない．
            if (!memo.contains(candidates_selected)) {
                combinationSumHelper(candidates, target - candidate, memo, candidates_selected);
            }

            // ここまで来れば，candidates_selectedに新たにcandidateを加えた組の和がtargetになるか判定済：なるならmemoに追加されていて，ならないならmemoは変更されない．
            // candidates_selectedは参照渡しなので，次のループのために今のcandidateを削除
            candidates_selected.erase(inserted_it);
        }
    }
};
```
```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());

        vector<int> candidates_selected;
        combinationSumHelper(candidates, target, 0, candidates_selected);

        return memo;
    }

private:
    vector<vector<int>> memo;

    void combinationSumHelper(vector<int>& candidates, int target, int start, vector<int>& candidates_selected) {
        // base case
        if (target == 0) {
            memo.push_back(candidates_selected);
            return;
        }

        // recursion step
        for (int i = start; i < candidates.size(); ++i) {
            // candidates[i]を足してcandidates_selectedの和がtargetを超えるなら何もせずcandidates[i+1]の調査へ．
            if (candidates[i] > target) break;

            candidates_selected.push_back(candidates[i]);
            combinationSumHelper(candidates, target - candidates[i], i, candidates_selected);
            candidates_selected.pop_back(); // iからi+1に移る前にcandidates[i]をメンバーから削除しておく．
        }
    }
};
```

# Step 2

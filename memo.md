# Step 1
- アナグラムは「文字の登場回数で見ると等しい」ということなので，「文字の登場回数」をキーにして，「文字の集合（配列）」をvalueに持つようなmapを構築すれば良い
  - `unordered_map`と`map`の比較．前者はハッシュマップ，平均計算時間は定数時間が多く最悪はO(N)が多い．後者は赤黒木，最悪計算時間はlog Nのオーダーが多い．
    - 使い分けについての私の理解は[こちら](https://github.com/5ky7/arai60/pull/9#discussion_r2083446939)
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        // 文字の登場回数でグループ分けしたいので，登場回数から文字列へのmapを作成
        map<vector<int>, vector<string>> num_letter_to_words;

        // strsの各要素を走査して，以上のmapに追加していく
        for (string str : strs) {
            vector<int> num_letter = getLetterCounter(str); // 各アルファベットの登場回数を取得

            // アナグラムとして初めて出てきたら，mapに新たに要素を追加．
            if (!num_letter_to_words.contains(num_letter)) {
                vector<string> word = {str};
                num_letter_to_words.insert({num_letter, word});
                continue;
            }
            // アナグラムとして既に存在していたら，valueにstrを追加
            vector<string> words = num_letter_to_words[num_letter];
            words.push_back(str);
            num_letter_to_words.erase(num_letter);
            num_letter_to_words.insert({num_letter, words});
        }
        
        vector<vector<string>> ans;
        for (pair<vector<int>, vector<string>> p : num_letter_to_words) {
            ans.push_back(p.second);
        }
        return ans;
    }

private:
    vector<int> getLetterCounter(const string& str) {
        vector<int> letter_counter(26,0); // letter_counter[0]がaの登場回数，letter_counter[25]がzの登場回数，
        for (char c : str) {
            int i = c - 97; // aのasciiコードは97．
            letter_counter[i]++;
        }
        return letter_counter;
    }
};
```

# Step 2
* アナグラムとして初登場か，既に登場しているかで場合分けする必要はないという点で上のコードは非効率的．修正したのが以下のコード
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        std::map<vector<int>, vector<string>> num_letters_to_words;

        for (const string& word : strs) {
            vector<int> num_letters = GetNumLetters(word);
            num_letters_to_words[num_letters].push_back(word);
        }

        vector<vector<string>> result;
        for (const auto& p : num_letters_to_words) {
            result.push_back(p.second);
        }
        return result;
    }

private:
    vector<int> GetNumLetters(const string& word) {
        vector<int> num_letters(26,0);
        for (char c : word) {
            ++num_letters[c - 'a'];
        }
        return num_letters;
    }
};
```

* アナグラムとして同一か判定する手法としてsort後の文字列を見る，というものがあったので実装してみる
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        std::map<string, vector<string>> sorted_to_words;

        for (const string& word : strs) {
            string sorted = GetSortedLetters(word);
            sorted_to_words[sorted].push_back(word);
        }

        vector<vector<string>> result;
        for (const auto& p : sorted_to_words) {
            result.push_back(p.second);
        }
        return result;
    }

private:
    string GetSortedLetters(string word) {
        std::sort(word.begin(), word.end());
        return word;
    }
};
```

* 例外処理を考える
  * アルファベット小文字以外が来た時の対応
    * エラーを返す --> `GetNumLetters()`のfor文内でチェックすれば良い
    * エラーを返さず，そのままアナグラム判定する --> 同一判定の方法を変える．`num_letters`の数値型にvector<int>ではなくmap<char, int>を用いる．
      * 計算量もほぼ変わらないし，この方法の方が良さそう．ということで実装．
      * 実行時間が（Leetcodeの測定が不正確という点を踏まえても）かなり増えてしまっている(30ms --> 71ms)のは気になる点．
        * 要素アクセスが定数（vector）からlog（map, 赤黒木）に変わる．
        * またある文字列の中で初めての文字が現れると，mapへの追加でlogオーダーの処理（vectorなら定数）
        * しかしアルファベット26文字しかないので，せいぜいlogの影響はlog 26 = 4.7程度で小さいと思った．
        * が，これが意外に大きい説はある．
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        std::map<std::map<char, int>, std::vector<string>> num_letters_to_words;

        for (const string& word : strs) {
            std::map<char, int> num_letters = GetNumLetters(word);
            num_letters_to_words[num_letters].push_back(word);
        }

        vector<vector<string>> result;
        for (const auto& p : num_letters_to_words) {
            result.push_back(p.second);
        }
        return result;
    }

private:
    std::map<char, int> GetNumLetters(const string& word) {
        std::map<char, int> num_letters;
        for (char c : word) {
            ++num_letters[c];
        }
        return num_letters;
    }
};
```

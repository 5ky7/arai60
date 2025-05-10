# Step 1
- アナグラムは「文字の登場回数で見ると等しい」ということなので，「文字の登場回数」をキーにして，「文字の集合（配列）」をvalueに持つようなmapを構築すれば良い
  - `unordered_map`と`map`の比較．前者はハッシュマップ，平均計算時間は定数時間が多く最悪はO(N)が多い．後者は赤黒木，最悪計算時間はlog Nのオーダーが多い．
  - 基本的には`map`を使うと良い，というか`unordered_map`は[使うな](https://groups.google.com/a/chromium.org/g/chromium-dev/c/rdxOHKzQmRY)という意見もある．
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

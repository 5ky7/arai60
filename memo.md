# Step 1
- sの各文字から回数へのmapを作って，もう一度sの先頭から見ていって回数が1のものを出力(39ms)．
```cpp
class Solution {
public:
    int firstUniqChar(string s) {
        std::map<char, int> char_to_count;
        for (char c : s) {
            ++char_to_count[c];
        }

        for (int i = 0; i < s.size(); ++ i) {
            if (char_to_count[s[i]] == 1) return i;
        }
        return -1;
    }
};
```
- `vector<int>`でもやってみる．
    - vectorの方は`c - 'a'`としてindexを取得しているので想定外の入力に弱い．
        - もちろんチェックして弾けば良いが，mapの方が大文字や特殊文字，ascii外の文字が来た時にも対応しやすい．
        - vectorだと文字の候補が増えるに従ってあらかじめ確保するメモリが増えるので，日本語とか入ると怪しくなってくる．
```cpp
class Solution {
public:
    int firstUniqChar(string s) {
        std::vector<int> char_to_count(26,0);
        for (char c : s) {
            ++char_to_count[c - 'a'];
        }

        for (int i = 0; i < s.size(); ++ i) {
            if (char_to_count[s[i] - 'a'] == 1) return i;
        }
        return -1;
    }
};
```
# Step 2
* queueを使ってやってみる（HashedLinkedListを使った解法はこれのことを指している？）
```cpp
class Solution {
public:
    int firstUniqChar(string s) {
        std::unordered_map<char, int> char_to_count;
        std::queue<LetterAndIndex> letters_appeared_once;
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            ++char_to_count[c];
            if (char_to_count[c] == 1) letters_appeared_once.push({c, i});

            while (!letters_appeared_once.empty()) {
                char first_letter = letters_appeared_once.front().letter;
                if (char_to_count[first_letter] == 1) {
                    break;
                }
                letters_appeared_once.pop();
            }
        }

        cout << letters_appeared_once.front().index << endl;
        return letters_appeared_once.empty() ? -1 : letters_appeared_once.front().index;
    }
private:
    struct LetterAndIndex {
        char letter;
        int index;
        LetterAndIndex(char c, int i) {
            letter = c;
            index = i;
        }
    };
};
```
* LRUの応用でもある．
    * 時間がある時に実装してみる．

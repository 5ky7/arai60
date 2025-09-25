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
- `unordered_map<char, int>`(20ms)，`vector<int>`(1ms)でもやってみた．
    - vectorの方は`c - 'a'`としてindexを取得しているので想定外の入力に弱い．
        - もちろんチェックして弾けば良いが，mapの方が大文字や特殊文字，asciiで'a'より前の文字が来た時にも対応できる
        - vectorだと文字の候補が増えるに従ってmemory consumingなので日本語とか入ると怪しくなってくる．
# Step 2

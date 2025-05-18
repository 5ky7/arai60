# Step 1
- sの各文字から回数へのmapを作って，もう一度sの先頭から見ていって回数が1のものを出力．
```cpp
class Solution {
public:
    int firstUniqChar(string s) {
        map<int, int> char_to_count;
        for (char c : s) {
            char_to_count[c]++;
        }
        for (int i = 0; i < s.size(); ++i) {
            if (char_to_count[s[i]] == 1) {
                return i;
            }
        }

        return -1;
    }
};
```
# Step 2

[3. Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters/description/?envType=problem-list-v2&envId=n97t3qfj)
# Step 1
とりあえず解いたのが[Code1](#Code1)．
#### Code1
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int start = 0;
        int end = 0;
        std::unordered_map<char, int> char_to_index;
        int max_len_substring = 0;
        while (end < s.size()) {
            char new_char = s[end];
            while (char_to_index.contains(new_char)) {
                char_to_index.erase(s[start]);
                ++start;
            }
            char_to_index[new_char] = end;
            max_len_substring = std::max(max_len_substring, end - start + 1);
            ++end;
        }
        return max_len_substring;
    }
};
```
mapでなくてsetで解けるじゃん，と思い[Code2](#Code2)．
#### Code2
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int start = 0;
        int end = 0;
        std::unordered_set<char> chars_in_substring;
        int max_len_substring = 0;
        while (end < s.size()) {
            char new_char = s[end];
            while (chars_in_substring.contains(new_char)) {
                chars_in_substring.erase(s[start]);
                ++start;
            }
            chars_in_substring.insert(new_char);
            max_len_substring = std::max(max_len_substring, end - start + 1);
            ++end;
        }
        return max_len_substring;
    }
};
```

mapを使うなら`start`の移動を効率化できる．[Code3](#Code3)．
#### Code3
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int start = 0;
        int end = 0;
        std::unordered_map<char, int> char_to_index;
        int max_len_substring = 0;
        while (end < s.size()) {
            char new_char = s[end];
            if (char_to_index.contains(new_char) && start <= char_to_index[new_char]) {
                start = char_to_index[new_char] + 1;
            }
            char_to_index[new_char] = end;
            max_len_substring = std::max(max_len_substring, end - start + 1);
            ++end;
        }
        return max_len_substring;
    }
};
```

# Step 2
---
setやmapを用いる代わりに，[substringに対してcontainsを用いる](https://github.com/5103246/LeetCode_Arai60/pull/45/changes#diff-221a2b21bf02260a855f6cf00ae981a1ad625db58659bdc18e2b6ad3bf6b1a1bR52)コードもあった．正確にはstringだとsubstrでコピーが走るのでstring_viewにしている．ただstring(_view)のcontainsは最悪で線形時間かかるからcontainsの実行は定数時間ではなくなる．にしてもO(n^3)で回るのはC++の強いところだし，その強みがあるからこそ実装でsliding windowが思い浮かばなくても計算時間を見積もっていけそうならシンプルな方法で実装するのも手であることは忘れないようにしたい．

こちらのコードの計算量は$`O(n^3)`$だと思っていたが，実際には`break`の打ち切りがあり，substringの長さは登場する文字の種類($`w`$とする)以上にはならないので$`O(nw^2)`$で抑えられる．

文字の種類数が事前にわかっていればmapの代わりに[vector(array)でも実装可能](https://github.com/5103246/LeetCode_Arai60/pull/45/changes#diff-221a2b21bf02260a855f6cf00ae981a1ad625db58659bdc18e2b6ad3bf6b1a1bR91)．
実質的にはハッシュテーブルと同じことをやっていて，ハッシュ関数の定義域と値域が同じケースと解釈可能．

---


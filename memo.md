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

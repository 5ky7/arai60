# Step 1
過去に解いてから時間が経ってので解き直してみる．再帰的にできそうなのでとりあえず書いてみて([Code1](#Code1)，その後でメモ化を試みる([Code2](#Code2))．

#### Code1
```cpp
#include <vector>

class Solution {
public:
    bool wordBreak(const string s, const vector<string>& wordDict) {
        return CheckConstractability(s, wordDict, is_constructable);
    }

private:
    bool CheckConstractability(const string s, 
                               const vector<string>& wordDict) {
        if (s.empty()) {
            return true;
        }

        for (string target : wordDict) {
            if (s == target) {
                return true;
            }

            // s != target
            if (s.size() < target.size()) {
                continue;
            }
            string prefix = s.substr(0, target.size());
            string suffix = s.substr(target.size());
            if (prefix != target) {
                continue;
            }
            if (CheckConstractability(suffix, wordDict, is_constructable)) {
                return true;
            }
        }
        return false;
    }
};
```


過去に解いたものが[PastCode](#PastCode)．個人的なメモに近いのでレビューはしていただかなくても良いかもしれません．

#### PastCode
```cpp
// 自力でやってみる
using namespace std;

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        // 辞書を文字数が長い順に整理．
        sort(wordDict.begin(), wordDict.end(),
            [](const string& a, const string& b){
                return a.size() > b.size();
            });

        map<string, bool> memo;
        return wordBreakHelper(s, wordDict, memo);
    }

    bool wordBreakHelper(string s, vector<string>& wordDict, set<string> memo) {
        if (s.empty() || memo.contains(s)) return true;

        for (string word : wordDict) {
            int len = word.size();
            if (s.substr(0,len) == word) {
                string sub_s = s.substr(len);
                if (!wordBreakHelper(sub_s, wordDict, memo)) {
                    map.
                    continue;
                memo.insert(sub_s);
                return true;
            }
        }

        return false;
    }
};
```

```cpp
// BFS
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        set<string> wordDict_set(wordDict.begin(), wordDict.end()); // wordDict.size() * wordDict内の長さの最大値　の計算量
        deque<int> founded;
        vector<bool> s_index_reachable(s.size() + 1, false);

        founded.push_back(0);

        while (!founded.empty()) {
            int start = founded.front();
            founded.pop_front();

            if (start == s.size()) return true;

            for (int end = start + 1; end <= s.size(); ++end) {
                if (s_index_reachable[end]) {
                    continue;
                }

                if (wordDict_set.find(s.substr(start, end - start)) != wordDict_set.end()) { // このsubstr取得でO(n)の計算量が生じることに注意
                    s_index_reachable[end] = true;
                    founded.push_back(end);
                }
            }
        }
        
        return false;
    }
};
```

```cpp
// top-down DP
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        memo = vector<int>(s.size(), -1);
        return is_constructible(s.size() - 1, s, wordDict);
    }
private:
    vector<int> memo;
    bool is_constructible(int i, const string& s, const vector<string>& wordDict) {
        if (i == -1) return true;
        
        if (memo[i] != -1) return memo[i] == 1;

        for (string word : wordDict) {
            int len = word.size();
            if (i + 1 - len < 0) continue;

            if (s.substr(i - len + 1, len) == word && is_constructible(i - len, s, wordDict)) {
                memo[i] = 1;
                return true;
            }
        }

        memo[i] = 0;
        return false;
    }
};
```

```cpp
// bottom-up dp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        vector<bool> is_constructable(s.size() + 1, false); // is_constructable[i] :「sのi文字目」までが構成可能か？(i=0は空文字列として常にtrue)
        is_constructable[0] = true;

        for (int i = 0; i <= s.size(); ++i) {
            // is_constructableをbottom-upに計算していく

            // sのi文字目までが構成可能でないならi+1以降も構成不可能
            if (!is_constructable[i]) continue;

            for (string word : wordDict) {
                if (i + word.size() > s.size()) continue;

                if (s.substr(i, word.size()) == word) {
                    is_constructable[i + word.size()] = true;
                }
            }
        }

        return is_constructable[s.size()];
    }
};
```

```cpp
// Trie
class Solution {
private:
    struct TrieNode {
        bool is_end = false;
        unordered_map<char, TrieNode*> children;
    };
    TrieNode* root = new TrieNode();
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        for (string& word : wordDict) {
            TrieNode* node = root;
            for (char c : word) {
                if (node->children.find(c) == node->children.end()) {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            node->is_end = true;
        }

        vector<bool> is_constructable(s.size() + 1, false);
        is_constructable[0] = true;

        for (int i = 0; i <= s.size(); ++i) {
            if (!is_constructable[i]) continue;

            TrieNode* node = root;
            for (int j = i; j < s.size(); ++j) {
                char c = s[j];
                if (node->children.find(c) == node->children.end()) break;

                node = node->children[c];
                if (node->is_end) {
                    is_constructable[j + 1] = true;
                }
            }
        }

        return is_constructable[s.size()];
    }
};
```

```cpp
// another dp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        vector<bool> is_constructable(s.size() + 1, false);
        is_constructable[0] = true;

        for (int i = 0; i < is_constructable.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (is_constructable[j] && words.contains(s.substr(j, i - j))) {
                    is_constructable[i] = true;
                    break;
                } // s = "abcde", words = {"ab", "bcd", "cde"}
            }
        }

        return is_constructable[s.size()];
    }
};
```

# Step 2

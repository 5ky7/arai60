# Step 1
　過去に解いてから時間が経ってので解き直してみる．再帰的にできそうなのでとりあえず書いてみて([Code1](#Code1)，その後でメモ化を試みる([Code2](#Code2))．

　メモ化を試みた際，単にメモを用意するだけではTLEだった．効率よくメモに書き込む必要があった．ここでの効率は，`wordDict`から文字数について貪欲に選ぶと言うことに対応していた．つまり，できるだけ早く`is_constructable_from`に書き込みをするべきで，それにはできるだけ早く末尾に近い（再帰呼び出しが起こらないことに対応）`start_pos`を引数にもつ`CheckConstructability()`の呼び出しに到達する必要がある．そのためには`start_pos`が大きくなる順に，つまり文字数が大きい順に`wordDict`から取り出すべきと言うことになる．

　このTLEは事前に見積もれるようになりたい．最悪のケースでは
- 入力:
    - `s = 'a' * 299 + 'b'`
    - `wordDict = {"a", "aa", ... , 'a'*20, (あとはaを使わずbを含まない適当な文字列を1980個)}`

この時，wordDictから取り出す順番を工夫しないと，300回の再帰呼び出しがあり，各呼び出しで2000回wordDictから1要素を取り出すループを回すことになる．要するに最悪で`s.size() * wordDict.size()`だけかかるわけで，$`6 \times 10^5`$ほどのステップを要する．

　この辺まで考えて，wordDictをhashmap的なものに入れて，`s`を走査してはhashmap lookupを行えばO(s.size())でできるんじゃないかと思った．[Code3](#Code3)．しかし冷静に考えればhashmapでヒットしたあと再帰呼び出しするんだから$`O(n^2)`$だった．一応まあメモ化してCode2と同様に$`O(n)`$に落としてみた([Code4](#Code4))．結局[Code2](#Code2)と似た感じに．違いはwordDictを走査するか，sを走査するか．
- と言うか，なんならCode2の方が平均的な入力に対しては走査数少なさそう．

#### Code1
```cpp
#include <vector>

class Solution {
public:
    bool wordBreak(const string s, const vector<string>& wordDict) {
        return CheckConstructability(s, wordDict);
    }

private:
    bool CheckConstructability(const string s, 
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
            if (CheckConstructability(suffix, wordDict)) {
                return true;
            }
        }
        return false;
    }
};
```

#### Code2
```cpp
#include <vector>

class Solution {
public:
    bool wordBreak(const string s, std::vector<string>& wordDict) {
        std::vector<int> is_constructable_from(s.size(), kNotChecked); // 0: not checked, 1: checked(not constructable from there)
        std::sort(wordDict.begin(), wordDict.end(), [](string& a, string& b) { return a.size() > b.size(); });
        return CheckConstructability(s, 0, wordDict, is_constructable_from);
    }

private:
    static constexpr int kNotChecked = 0;
    static constexpr int kNotConstructable = 1;

    bool CheckConstructability(const string s, 
                               const int start_pos,
                               const std::vector<string>& wordDict,
                               std::vector<int>& is_constructable_from) {
        if (start_pos == s.size()) {
            return true;
        }
        if (is_constructable_from[start_pos] == kNotConstructable) {
            return false;
        }

        string substring = s.substr(start_pos);
        for (string target : wordDict) {
            if (substring == target) {
                return true;
            }

            // s != target
            if (substring.size() < target.size()) {
                continue;
            }
            string prefix = substring.substr(0, target.size());
            string suffix = substring.substr(target.size());
            int new_start_pos = start_pos + target.size();
            if (prefix != target) {
                continue;
            }
            if (CheckConstructability(s, new_start_pos, wordDict, is_constructable_from)) {
                return true;
            }
        }
        is_constructable_from[start_pos] = kNotConstructable;
        return false;
    }
};
```
#### Code3
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        if (s.empty()) {
            throw std::invalid_argument("`s` must not be empty.");
        }

        std::unordered_set<string> word_dict(wordDict.begin(), wordDict.end());
        return CheckConstructability(s, word_dict);
    }

private:
    bool CheckConstructability(const string s, const std::unordered_set<string>& word_dict) {
        if (word_dict.contains(s)) {
            return true;
        }

        for (int partition_pos = s.size() - 1; 0 <= partition_pos; --partition_pos) {
            string prefix = s.substr(0, partition_pos);
            if (!word_dict.contains(prefix)) {
                continue;
            }
            string suffix = s.substr(partition_pos);
            if (CheckConstructability(suffix, word_dict)) {
                return true;
            }
        }
        return false;
    }
};
```

#### Code4
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        if (s.empty()) {
            throw std::invalid_argument("`s` must not be empty.");
        }

        std::unordered_set<string> word_dict(wordDict.begin(), wordDict.end());
        std::vector<int> is_constructable_from(s.size(), kNotChecked);
        return CheckConstructability(s, 0, word_dict, is_constructable_from);
    }

private:
    static constexpr int kNotChecked = 0;
    static constexpr int kNotConstructable = 1;

    bool CheckConstructability(const string s,
                               const int start_pos,
                               const std::unordered_set<string>& word_dict,
                               std::vector<int>& is_constructable_from) {
        string substring = s.substr(start_pos);
        if (word_dict.contains(substring)) {
            return true;
        }
        if (is_constructable_from[start_pos] == kNotConstructable) {
            return false;
        }

        for (int prefix_length = substring.size(); 1 <= prefix_length; --prefix_length) {
            string prefix = s.substr(start_pos, prefix_length);
            if (!word_dict.contains(prefix)) {
                continue;
            }
            if (CheckConstructability(s, start_pos + prefix_length, word_dict, is_constructable_from)) {
                return true;
            }
        }
        is_constructable_from[start_pos] = kNotConstructable;
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

　[正規表現との関連付け](https://discord.com/channels/1084280443945353267/1200089668901937312/1221644164576444527)．正規表現はオートマトンと対応するから，適切なオートマトンを導入して入力を先頭から1文字ずつ入れていった最後の状態がゴール状態かどうかで判定できる．つまり一度舐めれば判定できるはず．

しかし書いてあるコードを読んでも，最終的にDFS的になっているのはわかるが，あんまりオートマトンという解釈ができない．[こちら](https://discord.com/channels/1084280443945353267/1200089668901937312/1222020658955816960)をみても同様．なんか入力を一つずつ入れている感覚がない．これは`s`を舐めているのか？

　いずれにせよ変数の置き方や条件の立て方などは参考になったので，書き直してみる．[Code5](#Code5)．

#### Code5
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        std::stack<int> frontier; 
        // frontier: `s`のindexで，まだそこからwordDictの要素を用いて到達できるindexを調べていないものの集合．
        std::set<int> reachable;
        // reachable : wordDictによってs.substr(0, i)が構成可能となるようなiの集合．
        frontier.push(0);
        reachable.insert(0);

        while (!frontier.empty()) {
            int start = frontier.top();
            frontier.pop();
            for(string word : wordDict) {
                int last = start + word.size();
                // 条件に当てはまらないものを除く
                if (reachable.contains(last)) {
                    continue;
                }
                if (s.substr(start, word.size()) != word) {
                    continue;
                }

                // この時点で`s.substr(0, last)`はwordDictから構成可能．
                if (last == s.size()) {
                    return true;
                }
                reachable.insert(last);
                frontier.push(last);
            }
        }
        return false;
    }
};
```

---
[Code5](#Code5)の別バリエーションが[Code6](#Code6)．whileループ内で`wordDict`を走査しては`s`の部分文字列に対応するものがあるかチェックする部分があったが，これを`s`を走査しては`wordDict`に対応するものがあるかチェックする方式に．時間計算量が$`O(nmk)`$から$`O(n^2)`$に．ただし$`n = \text{s.size()}, \, m = \text{wordDict.size()},\, k = \text{max\_len(wordDict)}`$

今回の場合は$`km <= 20000, \, n <= 300`$なのでCode6の方が最悪計算時間は短いはず．
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        std::unordered_set<string> word_dict(wordDict.begin(), wordDict.end());
        std::stack<int> frontier; 
        // frontier: `s`のindexで，まだそこからwordDictの要素を用いて到達できるindexを調べていないものの集合．
        // frontierの要素iに対し，s[i]より前は構築可能であることが要請される．
        std::set<int> reachable;
        // reachable : wordDictによってs.substr(0, i)が構成可能となるようなiの集合．
        // 
        frontier.push(0);
        reachable.insert(0);

        while (!frontier.empty()) {
            int start = frontier.top();
            frontier.pop();
            string prefix = "";
            for (int end = start + 1; end <= s.size(); ++end) {
                prefix += s[end - 1];
                if (reachable.contains(end)) {
                    continue;
                }
                // prefix : s[start]からs[end - 1]までからなる部分文字列
                if (word_dict.contains(prefix)) {
                    reachable.insert(end);
                    frontier.push(end);
                }
            }
        }
        if (reachable.contains(s.size())) {
            return true;
        }
        return false;
    }
};
```

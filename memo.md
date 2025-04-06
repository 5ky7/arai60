# Step 1
- かっこは最後につけたものから閉じていかないといけないのでスタックで
  - と言いつつ，arai60の"stack"にあった問題なので頭にスタックが浮かんではいた
- 始めかっこの場合はpush
- 終わりかっこの場合は，
  - スタックが空なら対応する始めかっこがないので`return false`
  - 空でないならtop()が終わりかっこに対応しているかcheckし，対応していればpop()する．していなければ`return false`
- これを入力文字列の初めから終わりまで走査して行なったのち，
  - スタックが空ならreturn true
  - それ以外は閉じれていないかっこがあるのでreturn false
```c++
#include <stack> 
using std::stack;
using std::map;

class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        int LenStr = s.size();
        map<char, char> pair;
        pair['('] = ')';
        pair['['] = ']';
        pair['{'] = '}';

        for (int i=0; i<LenStr; i++) {
            if (s.at(i) == '(' || s.at(i) == '[' || s.at(i) == '{') {
                st.push(s.at(i));
                continue;
            }
            if (s.at(i) == ')' || s.at(i) == ']' || s.at(i) == '}') {
                if (st.empty()) {
                    return false;
                }
                if (pair[st.top()] != s.at(i)) {
                    return false;
                }
                st.pop();
            }
        }
        if (st.empty()) {
            return true;
        }
        return false;
    }
};
```
# Step 2

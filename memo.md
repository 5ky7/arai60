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
- 初期化リストを用いれば辞書`pair`の作成はもっとシンプルにできる
```c++
map<char, char> pair = {
  {'(', ')'},
  {'[', ']'},
  {'{', '}'}
};
```
- 一応，オーダー的な計算量ではunordered_mapのほうが速いとされるが，Nが小さくunordered_mapによる高速化の恩恵もわずかだろうし，何も考えずに使うならmapのが良いということでmapにした
- `if (s.at(i) = <とじかっこ>)`はいらない．`s.at(i)`が開きかっこ出なければ必ずとじかっこなので．
- stackの底にdummyをおくと`for`ループ内の`if (st.empty())`は不要になる．
  - しかもdummyの中身をヌル文字にしておけば判定時に`return true`を条件づけて返すのではなく，`return !st.top()`のみで済む！
- 変数名`st`を`st_open_brackets`に変更．
- for文を拡張forに変更．（以上までで，以下のコード）
  - カウンタ変数の名前をどうするか迷った．意味的にbracketにすると，変数名が長くなって読みにくい一方で，大規模化したときには"bracket"がいろんなところでいろんな意味で使われうるので意味が分かりにくそう．bracket_in_sとかにするのが良いか？
  - 今回は短いfor文の中でのみ使うので視認性を重視して1文字変数にしたが，これが良いかはわからない．みなさまの意見お待ちしています．
  - 「意味がすぐわかり，かつすぐ忘れても良い変数なら，一文字でも良い」という意見があったので一旦納得．あくまで伝わるか伝わらないかの話．
```c++
class Solution {
public:
    bool isValid(string s) {
        stack<char> st_open_brackets;
        st_open_brackets.push('\0');
        map<char, char> pair = {
            {'(', ')'},
            {'{', '}'},
            {'[', ']'}
        };

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                st_open_brackets.push(c);
                continue;
            }
            if (pair[st_open_brackets.top()] != c) {
                return false;
            }
            st_open_brackets.pop();
        }
        return !st_open_brackets.top();
    }
};
```
- LeetCode上ではあり得ないが，実務上変な入力が来たらどうするか，は意識すべき．
- 入力として`(hoge)(fuga)`みたいなのが来たらどうするか？-->次の2通りの仕様が考えられる．
  - hogefugaのところは無視して，括弧のみに注目したら閉じているから`true`を返す（下のコード）
  - `h`の時点でエラーを投げる（下の下のコード）
```c++
class Solution {
public:
    bool isValid(string s) {
        stack<char> st_open_brackets;
        st_open_brackets.push('\0');
        map<char, char> pair = {
            {'(', ')'},
            {'{', '}'},
            {'[', ']'}
        };

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                st_open_brackets.push(c);
                continue;
            } else if (c == ')' || c == ']' || c == '}') {
                if (pair[st_open_brackets.top()] != c) {
                    return false;
                }
                st_open_brackets.pop();
            }
        }
        return !st_open_brackets.top();
    }
};
```
```c++
class Solution {
public:
    bool isValid(string s) {
        stack<char> st_open_brackets;
        st_open_brackets.push('\0');
        map<char, char> pair = {
            {'(', ')'},
            {'{', '}'},
            {'[', ']'}
        };

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                st_open_brackets.push(c);
                continue;
            } else if (c == ')' || c == ']' || c == '}') {
                if (pair[st_open_brackets.top()] != c) {
                    return false;
                }
                st_open_brackets.pop();
            } else {
                cerr << "invalid input" << endl;
                exit(EXIT_FAILURE); // プログラムを終了
            }
        }
        return !st_open_brackets.top();
    }
};
```
- 「条件式の部分に，何かを変えるような作用がある式は入れたくない」←確かに．if内は実行しないのに条件文の評価によって変数が変わったりしたらややこしい（あえてうまく使える場面もありそうではある）．
- `pair`は定数で変化させないので，安全のためconstをつけても良い．constexprがつけられたら高速化にも繋がるが，mapにはつけられない．

  

- チョムスキー階層とかプッシュダウンオートマトンについて．多分単発で学ぶより体系的に学ぶべきだろうから，CSZAPの参考図書を読んでみる（というかできたらCSZAPに参加したい）
  - ちょうど形式言語，システムデザイン，プログラミング言語各論については全く触れていないので，この辺は単発記事を読むだけじゃ理解できなさそう
- 再帰下降構文解析は[こちらのサイト](https://dai1741.github.io/maximum-algo-2012/docs/parsing/)がわかりやすかったが，本問での実装はうまくいかなかった．
  - 深すぎる再帰によるスタックオーバーフローに注意．
- [他のPR](https://github.com/fhiyo/leetcode/pull/6)を見てもなぜそれでうまくいくのか分からず，何も見ずにAcceptされるコードを書くこともできずにいる．一旦後回しにする．
- ラムダ式 vs 独立関数
  - ローカル状態を引数で渡す必要がないので，その点でラムダが楽．
  - 小さな局所ロジックはラムダで定義した方が可読性が高い．一方汎用的に使い回すなら独立関数として定義するのが良い．
  

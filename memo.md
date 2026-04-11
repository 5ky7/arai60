# Step 1
- 重複なしだから`set`を使おう．
  - メールアドレスを保持するシステムだったら要素数が少ない場合が多そうだから`unorderes_set`より`set`の方が良い．（[参照](https://discord.com/channels/1084280443945353267/1345977675251978351/1371036057574969414)）
- 場合分けの書き方の候補として
  - `if ... else if ...` で最後にのみ`processed_email.push_back(email[i]);`を入れる．`c == ','`の場合などは`continue`して`push_back`をさせない．
  - `if ... else if ... else ...` で各分岐に`processed_email.push_back(email[i]);`を入れる．`c == ','`の場合などは分岐の中に`push_back`を入れない
  - `if () {... continue;} if () {... continuie;}`で書く．各分岐で`processed_email.push_back(email[i]);`を入れたり入れなかったりする．最後に分岐に引っ掛からなかった場合（`c`が普通のアルファベットの場合）として`processed_email.push_back(email[i]);`を入れる
- [目の移動量を減らす](https://docs.google.com/document/d/11HV35ADPo9QxJOpJQ24FcZvtvioli770WWdZZDaLOfg/edit?tab=t.0#heading=h.emwxn8guot8x)には，1つ目はよくない
- 今回のプログラムは[「例外を除いて，残りを処理する」](https://discord.com/channels/1084280443945353267/1295357747545505833/1297735299618705408)に対応するので3つ目を採用．

```cpp
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        std::set<std::string> emails_no_duplicates;

        for (std::string email : emails) {
            bool is_local_name = true;
            std::string processed_email;
            for (int i = 0; i < email.size(); ++i) {
                if (email[i] == '@') {
                    processed_email.push_back('@');
                    is_local_name = false;
                    continue;
                }
                if (email[i] == '.' && is_local_name) {
                    continue;
                }
                if (email[i] == '+' && is_local_name) {
                    while (email[i] != '@') ++i;
                    processed_email.push_back('@');
                    is_local_name = false;
                    continue;
                }
                processed_email.push_back(email[i]);
            }

            emails_no_duplicates.insert(processed_email);
        }

        return emails_no_duplicates.size();
    }
};
```

# Step 2
* domain_nameとlocal_nameの処理をネストすることで明確に分けた方が可読性が高そう([cf.](https://github.com/hayashi-ay/leetcode/pull/25/files?short_path=d65d436#diff-d65d43698547a0f3cfcdb7f005de30ed4cd0c45ae015fd01094d6647cfa0a84a))．ということで実装してみた．
  * `'@'`の後は一括で追加することで速くなってそうなのに，Step 1のコード(実行時間1ms)より遅い(実行時間7ms)のはleetcodeの実行時間の不正確さゆえか，それとも実際に遅くなっているのか．
    * 1文字ずつ追加すると，追加のたびにメモリの再確保（計算時間O(stringのサイズ)）が走るので結果O(stringのサイズ^2)でとても遅いはず．
    * メアドの長さ的に高々40文字程度だろうか．O(40)がO(1600)になるのは結構影響が大きそうではある．
  * 保守性を上げ，かつ無駄なコピーを避けるためにforのループ変数の型にconstと&をつけておく．
    * [レジスター幅](https://github.com/colorbox/leetcode/pull/28/files#r1845466651)は64bit cpuで8byte程度（-->intやdoubleなら参照渡しにする必要はない）
```cpp:nested.cpp
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        std::set<std::string> emails_no_duplicates;

        for (const std::string& email : emails) {
            bool is_local_name = true;
            std::string processed_email;
            for (int i = 0; i < email.size(); ++i) {
                if (is_local_name) {
                    if (email[i] == '@') {
                        processed_email.push_back('@');
                        is_local_name = false;
                        continue;
                    }
                    if (email[i] == '.') {
                        continue;
                    }
                    if (email[i] == '+') {
                        while (email[i] != '@') ++i;
                        processed_email.push_back('@');
                        is_local_name = false;
                        continue;
                    }
                    processed_email.push_back(email[i]);
                } else {
                    // domain_nameはそのまま追加
                    processed_email.append(email, i);
                }
            }

            emails_no_duplicates.insert(processed_email);
        }

        return emails_no_duplicates.size();
    }
};
```
* と思ったが，このコード**`else`の最後で`break`してない**からforループが回るたびにappendしてないか？なんで通るんだ？
  * デバッガで確認したら`emails_no_duplicates:{[0] = "testemail@lee.tcode.comee.tcode.come.tcode.com.tcode.comtcode.comcode.comode.comde.come.com.comcomomm", [1] = "testemail@leetcode.comeetcode.cometcode.comtcode.comcode.comode.comde.come.com.comcomomm"}`．やっぱり判定がおかしい．
  * もちろんbreak;をつけてもAccepted，
  * そして実行時間は0msに．やはり無駄なappendのせいで時間食ってたみたい．
* `email`から新しい文字列`processed_email`を作る方針から，`email`を直接編集する方針でやってみる．
  * `@`が出たら残りはそのまま，とか，push_back()は必要ない，とか，処理の数は減らせそう．
  * 一方で可読性は`processed_email`を用意した方が高いと思う．
* というわけで実装したが10倍くらい遅くなっている
  * 考えてみればvectorに対するeraseって消した要素より後ろの要素を一つずつ前に持ってこなきゃいけないわけで，結構時間かかりそうだ．
    * と思って調べてみたら要素が[POD](https://rinatz.github.io/cpp-book/ch12-02-plain-old-data/)なら一つの要素移動に数サイクル（算術演算と同等），かつキャッシュヒットすれば全体の移動も実質算術演算と同等．よって今回は対して遅くならない（算術演算と同程度）とのこと．
      * ここはちょっとわからなかった．キャッシュヒットしてても数サイクル * 要素数だけの演算は必要では？
  * ここは入力されるアドレスの性質（`'+'`の後ろの文字や`'.'`の多寡，domain_nameの平均的な長さ）によるか．
  * 今回，こっちの方が遅いということはeraseに時間がかかっている（=`'.'`が多い，メアドが長い，など）ということか．
```cpp:pointer.cpp
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        std::set<std::string> unique_email_addresses;

        for (std::string email : emails) {
            auto email_pointer = email.begin();
            while (*email_pointer != '@') {
                if (*email_pointer == '.') {
                    email.erase(email_pointer);
                    continue;
                }
                if (*email_pointer == '+') {
                    while (*email_pointer != '@') {
                        email.erase(email_pointer);
                    }
                    break;
                }
                ++email_pointer;
            }

            unique_email_addresses.insert(email);
        }

        return unique_email_addresses.size();
    }
};
```
* 今回の私のコードには登場しなかったが，戻り値がstringの関数は[要注意](https://github.com/colorbox/leetcode/pull/28/files#r1845466871)
  * C++17以降ではRVOが走るので無駄なコピーはあまりないが，それ以前では
    * [入力をコピーして破壊的に変更したものを返す](https://github.com/colorbox/leetcode/pull/28#discussion_r1844942851)
    * [文字列を外から参照で受け取って文字列を破壊的に変更して文字列の参照を返す](https://github.com/colorbox/leetcode/pull/28/files#r1844938665)
    * [返り値をvoidにして引数にoutputの参照を与える](https://github.com/colorbox/leetcode/pull/28/files#r2035185457)
  * などの方法がある．
* 例外処理の対応を考慮していない．以下の項目くらいはチェックしたい：
  * `'@'`が1つか
  * その前後が空文字でないか
* 今のままだと最初の@以降は変更することなく付け加えられる（e.g. ho.ge+geho@fuga.com@piyo+puyo.jp --> ho.ge@fuga.com@piyo+puyo.jp）
  * これが正当なメールアドレスになることはなさそうなので，不正な入力と認識してエラーを返すのが現実的だと思う．
  * 一方で残りのメールについては正しく処理しておきたい気もする．
    * 不正なメアドはunique_email_addressesに追加しないで処理を続けるか，それに加えて不正なメアドを保持する箱を用意しておいてそこに入れておく，あたりか．
  * [データサイエンス的なバッチ処理では落ちて欲しいこともある](https://github.com/shintaroyoshida20/leetcode/pull/20#discussion_r2079714768)とあり，納得．

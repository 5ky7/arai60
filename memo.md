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
* `email`から新しい文字列`processed_email`を作る方針から，`email`を直接編集する方針でやってみる．
  * `@`が出たら残りはそのまま，とか，push_back()は必要ない，とか，処理の数は減らせそう．
  * 一方で可読性は`processed_email`を用意した方が高いと思う．
* というわけで実装したが10倍くらい遅くなっている
  * 考えてみればvectorに対するeraseって消した要素より後ろの要素を一つずつ前に持ってこなきゃいけないわけで，結構時間かかりそうだ．
    * と思って調べてみたら要素が[POD](https://rinatz.github.io/cpp-book/ch12-02-plain-old-data/)なら一つの要素移動に数サイクル（算術演算と同等），かつキャッシュヒットすれば全体の移動も実質算術演算と同等．よって今回は対して遅くならない（算術演算と同程度）とのこと．
      * ここはちょっとわからなかった．キャッシュヒットしてても数サイクル * 要素数だけの演算は必要では？
  * ここは入力されるアドレスの性質（`'+'`の後ろの文字や`'.'`の多寡，domain_nameの平均的な長さ）によるか．
  * 今回，こっちの方が遅いということはeraseに時間がかかっている（=`'.'`が多い，メアドが長い，など）ということか．
```cpp
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

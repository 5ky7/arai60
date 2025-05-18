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

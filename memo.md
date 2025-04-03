# Step1
- とりあえず書いたのが次のコード
  - 出力を構築する用のポインタ`sol`と重複するかチェックするためのポインタ`node`を用意し，以下を繰り返す
    - `node->next`と`node`の中身が
      - 等しいなら初めて異なる値になるまでまで`node`をすすめる
      - 異なるなら`node`を`sol`に加える
  - Wrong Answerである．
    - `while (node)`だと処理の最後に`node`がnullptrになるはずだが，その時`while (node)`内の`else`が実行されない．
      - 例えば，入力が`[1,2,2]`だと，一旦要素`{val = 1, next = (次の2をvalにもつ要素のポインタ)}`の後に`next`をnullptrにする処理が行われないので，2が出力に含まれてしまう．
```C++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0);
        ListNode *sol = &dummy;
        ListNode *node = head;
        while (node) {
            if (node->next && node->next->val == node->val) {
                while (node->next && node->val == node->next->val) {
                    node = node->next;
                }
                node = node->next;
                continue;
            }
            else {
                sol->next = node;
                sol = sol->next;
                node = node->next;
            }
        }
        return dummy.next;
    }
};
```
- そこで修正したのが以下のコード
```C++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0);
        ListNode *sol = &dummy;
        ListNode *node = head;
        while (true) {
            if (node && node->next && node->next->val == node->val) {
                while (node->next && node->val == node->next->val) {
                    node = node->next;
                }
                node = node->next;
                continue;
            } else {
                sol->next = node;
                sol = sol->next;
                if (node) {
                    node = node->next;
                } else {
                    break;
                }
            }
        }
        return dummy.next;
    }
};
```

# Step2
- 1つ目の`if`とその中の`while`で`node->next`の存在確認が被っていたので`if`の方を削除
  - と思ったが，条件文の中で`node->next`の中身へのアクセスがあるから消せない．
  - 整理できそうだけど`while`の外の`node = node->next`が邪魔で`if`を`while`に置き換えるのもできないし，で悩み中
  - と思ったらコメント集に「フラグを立てる」ことで長い条件文の重複を回避する方法を発見．
```c++
            bool is_duplicated = false;
            while (node->next && node->val == node->next->val) {
                node = node->next;
                is_duplicated = true;
            }
            if (is_duplicated) {
                node = node->next;
                continue;
            } else {
                sol->next = node;
                sol = sol->next;
                if (node) {
                    node = node->next;
                } else {
                    break;
                }
            }
```
- `else`の中身って絶対通る（verifiedへの付け加えなので必須）から，`else`はなくて良いことに気づいた．
  - このコードを改めて説明すると，
    - 入力列を先頭から見ていく．
    - 注目ノードとその次のノードの値を比較する(*)
      - 等しいなら注目ノードを新しい値が出てくるまで進めて比較フェーズ(*)に戻る
      - 等しくないなら，出力列の末尾に注目ノードを付け加える．この時，
        - 注目ノードが入力列の末尾でないなら注目ノードを一つ進めて比較フェーズ(*)に戻る
        - 注目ノードが入力列の末尾（正確には末尾の次，すなわちnullptr）なら出力列を返してプログラム終了
- 変数名について，`sol`，`node`だと意味が分かりにくいので`verified`と`checking`に変更．（ここまでで以下のコード）
```　c++:step2-1.cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0);
        ListNode *verified = &dummy;
        ListNode *checking = head;
        while (true) {
            if (checking && checking->next && checking->val == checking->next->val) {
                while (checking->next && checking->val == checking->next->val) {
                    checking = checking->next;
                }
                checking = checking->next;
                continue;
            }
            verified->next = checking;
            verified = verified->next;
            if (checking) {
                checking = checking->next;
            } else {
                break;
            }
        }
        return dummy.next;
    }
};
```
- 「簡単な方から説明すると分かりやすい」(cf. コメント集）より，`if`文を「重複がある時」ではなく「重複がない時」に変更．
- 分岐の共通部分を抽出し，分岐合流後に実行．
- 重複している時の進め方を変更（ここまでで以下のコード）
- どちらが"簡単"であるかは，議論の余地あり（どちらのコードが読みやすいか，意見あればお願いします）
```　c++:step2-2.cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0);
        ListNode *verified = &dummy;
        ListNode *checking = head;
        while (true) {
            if (!checking || !checking->next || checking->val != checking->next->val) {
                verified->next = checking;
                verified = verified->next;
            } else {
                while (checking->next && checking->val == checking->next->val) {
                    checking = checking->next;
                }
            }
            
            if (checking) {
                checking = checking->next;
            } else {
                break;
            } 
        }

        return dummy.next;
    }
};
```
- 実は`verified`は使わなくても良い（以下のコード．checking->nextを操作する．）．
- が，可読性からは`verified`と`checkingを使った方が良さそう（cf. 「自然言語で説明するときに『注目ノードの次の値と注目ノードの次の次の値が等しい』というより『注目ノードの値と注目ノードの次の値が等しい』という方が自然」「checking->nextが中心になるならそれを新たな変数としておく」）．
- 入力リストから重複を切り取っていったものを出力とするのか（このコード），入力リストから重複しない要素を取り出して付け加えた新しいリストを出力とするのか（上記のコードたち）の違いとも言えそう．
```c++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0, head);
        ListNode *checking = &dummy;
        while (checking->next) {
            if (!checking->next->next || checking->next->val != checking->next->next->val) {
                checking = checking->next;
                continue;
            }
            int value_to_remove = checking->next->val;
            while (checking->next && checking->next->val == value_to_remove) {
                checking->next = checking->next->next;
            }
        }
        return dummy.next;
    }
};
```

# Step3

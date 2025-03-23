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
- 変数名について，`sol`，`node`だと意味が分かりにくいので`verified`と`checking`に変更．
```C++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0);
        ListNode *sol = &dummy;
        ListNode *node = head;
        while (true) {
            if (node && node->next->val == node->val) {
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

# Step3

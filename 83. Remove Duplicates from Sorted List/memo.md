# step1
- 2つのポインタ`p1`と`p2`を用意して，`p1->val` と `p2->val`が違う値になるまで`p2`を進めた後，`p1 = p2`をしようと考えた．
  - 今回からコードに出てくるものはインライン表示するようにしてみました．Markdownの書き方へのコメントも歓迎しています！
- が，出力が入力のままになっており，何がおかしいのかと悩んだ末，Chat GPTに相談
- 誤りは`if () {} else if () {} `の`else`を抜いて，　`if () {} if () {}`としたことが原因
- 一方，ポインタは1つで良いことも指摘を受ける．確かに．
  - `p->val != p->next->val` なら `p->next = p->next->next` とすれば良い．
- と言うわけでポインタを一つにして書き直した結果，Accepted.

```C++

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode *p = head;
        while (p != nullptr && p->next != nullptr) {
            if (p->val != p->next->val) {
                p = p->next;
                continue;
            }
            else {
                p->next = p->next->next;
                continue;
            }
        }
        return head;
    }
};

```
# step2
- `p != nullptr`っていうけど，条件式として結局`p`と同じなので`while`の条件部はもっと簡単にかける
  - どっちの方が可読性高いかはわからない．
- もしかして `p != nullptr` っていらない？
  - 入力が空とかならアレだけど，`while loop`ではpを1つ進めるか，`p->next`を一つ進めるかしかしないので，最初の時点で `p != nullptr` ならわざわざ毎回チェックしなくて良さそう．実際acceptされる．
  - ただわざわざ head != nullptr の条件分岐を作る点でコードは長くなる．どっちがいいか，意見お待ちしています．
- 以前変数名の指摘を受けたことを思い出し, `p`を`node`に修正．
- continueを使うと読む方が楽って見て，一瞬「本当か？」って思ったけど本当だった．すごい．パクらせていただきます．
  - しかし今回のコードではそこまで脳のメモリを食わない感じもするので，蛇足になっている気がしなくもない．ここも意見お待ちしています．
```C++

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode *node = head;
        if (!node) {
            return head;
        }
        while (node->next) {
            if (node->val != node->next->val) {
                node = node->next;
            }
            else {
                node->next = node->next->next;
            }
        }
        return head;
    }
};

```
- `while`文の別の書き方で良いものがあった
  - 上のコードは「今と次で値が違うなら注目ノードを一つ先に進める，値が同じなら注目ノードのnextを2つ先に進める」
  - 別の書き方として「今と次が異なるまで注目ノードのnextを勧めてから，注目ノードを一つ進める」（以下のコード）
```C++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode *node = head;
        if (!node) {
            return head;
        }
        while (node) {
            while (node->next && node->val == node->next->val) {
                node->next = node->next->next;
            }
            node = node->next;
        }
        return head;
    }
};

# step3
- 2種類のwhileを書いてそれぞれaccepted

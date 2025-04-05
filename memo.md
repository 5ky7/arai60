# Step 1
- 問題となるのは繰り上がりの処理
- 繰り上がりが生じる度に都度処理する方針で書いたのが以下のコード．
  - 番兵`dummy`を用意
  - `l1`,`l2`は各桁を指す．桁が存在する間，
    - `l1`,`l2`（存在すれば）の値，及び繰り上がりを足し算．
    - 足し算の結果が
      - 10未満であれば，足し算の結果を出力リストの要素に格納
      - 10以上であれば，10引いた値を出力リストの要素に格納し，繰り上がりとして出力リストの次の要素の値を1にセット
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *digit = &dummy;
        int sum;
        while (l1 || l2) {
            //if there was no carry up, add new digit to the output list;
            if (!digit->next) {
                digit->next = new ListNode(0);
            }
            digit = digit->next;

            if (l1 && l2) {
                sum = digit->val + l1->val + l2->val;
                l1 = l1->next;
                l2 = l2->next;
            } else if (l1) {
                sum = digit->val + l1->val;
                l1 = l1->next;
            } else if (l2) {
                sum = digit->val + l2->val;
                l2 = l2->next;
            }
            
            if (sum < 10) {
                digit->val = sum;
            } else {
                digit->val = sum - 10;
                digit->next = new ListNode(1);
            }

        }

        return dummy.next;
    }
};
```
- 短縮できそうな箇所を探す．
  - 最初に`digit->next`用のListNodeを作ってるけど，最後に`sum`が確定してから作れば良い．
  - `l1`,`l2`の有無でいちいち分岐（完全場合分け）させてるけど，「あれば足す」で書けばもっと短い
  - 最後に`sum`と10の大小で分岐させてるけど，繰り上がりを格納する変数`carry`を導入して最初に`sum = carry`とすれば分岐の必要がない．
- ということで出来上がったのが以下のコード
  - ただしこっちは実行時間3ms．上のコードは1ms以下とのこと．なぜ？（使用メモリは両者77MB程度で近しい）
    - オーダー表記上の計算量はどちらも
$$O\(\max \( \text{len}(l1),\text{len}(l2) \) \)$$
    - 下のコードの方が時間がかかる原因として考えられるものは
      - `while`での`carry`の判定の有無
      - `carry`への代入

```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *digit = &dummy;
        int sum, carry = 0;
        while (l1 || l2 ||carry) {
            sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }

            carry = sum / 10;
            sum = sum % 10;
            digit->next = new ListNode(sum);
            digit = digit->next;
        }

        return dummy.next;
    }
};
```
- 一旦繰り上がりを無視して足し算して，最後に整形するのが以下のコード.
  - 作業を分けて行なっている分，わかりやすいかなあと思って書いてみたが，今回は別にわかりやすくなっていなさそう．
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *digit = &dummy;
        int sum;

        while (l1 || l2) {
            sum = 0;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }

            digit->next = new ListNode(sum);
            digit = digit->next;
        }

        digit = dummy.next;
        while (digit) {
            if (digit->val > 9) {
                digit->val -= 10;
                if (digit->next) {
                    digit->next->val++;
                } else {
                    digit->next = new ListNode(1);
                }
            }
            digit = digit->next;
        }

        return dummy.next;
    }
};
```
# Step 2
- `digit`はわかりにくいかな．`checking_digit`に変更
- Tail recursionというものを知る．それで実装してみる．
  - `addTwoNumbers_recur(node->next, l1->next, l2->next, sum / 10)`みたいにできたら気分がいいなあと思ったが，`l1`が`nullptr`である可能性からして仕方ない
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;
        ListNode *checking = &dummy;
        
        addTwoNumbers_recur(checking, l1, l2, 0);

        return dummy.next;
    }

    void addTwoNumbers_recur(ListNode *node, ListNode *l1, ListNode *l2, int carry) {
        if (!l1 && !l2 && !carry) {
            return;
        }
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }

        node->next = new ListNode(sum % 10);
        addTwoNumbers_recur(node->next, l1, l2, sum / 10);
    }
};
```
- この再帰，なんか違和感あるなあと思ったが，私が`x[i] = recursion_func(x[i-1])`みたいな形の使い方しか見てこなかったからかもしれない．返り値を`carry`に対応させればそのような再帰にもできる．
- dummyを使わない方法もあるが，`head`がnullかどうかの条件分岐を入れる必要がある．私はdummyを使った方が好み．
- 2つに限らず複数の値の足し算の関数を作って，そのインスタンスとして解いてみる（下のコード）．
  - `addTwoNumbers`と`addNumbers`の返り値とか引数が合ってないのは気持ち悪いなと思って修正したところ（下の下のコード），なぜか実行時間が3msから7ms程度まで増加
```c++
#include <vector>
using std::vector;

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy;

        vector<ListNode*> myLists = {l1, l2};
        addNumbers(myLists, dummy);
        return dummy.next;
    }

    void addNumbers(vector<ListNode*>& lists, ListNode& dummy) {
        ListNode *checking = &dummy;
        int sum, carry = 0;

        while (is_there_empty_ptr(lists) || carry) {
            sum = carry;
            calcSum(lists, sum);

            checking->next = new ListNode(sum % 10);
            checking = checking->next;
            carry = sum / 10;
        }
    }

    bool is_there_empty_ptr(vector<ListNode*>& lists) {
        int len = lists.size();
        for (int i=0; i<len; i++){
            if (lists.at(i)) {
                return true;
            }
        }
        return false;
    }

    void calcSum(vector<ListNode*>& lists, int& sum) {
        int len = lists.size();
        for (int i=0; i<len; i++){
            if (lists.at(i)) {
                sum += lists.at(i)->val;
                lists.at(i) = lists.at(i)->next;
            }
        }
    }
    

};
```
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        vector<ListNode*> myLists = {l1, l2};
        return addNumbers(myLists);
    }

    ListNode* addNumbers(vector<ListNode*>& lists) {
        ListNode dummy;
        ListNode *checking = &dummy;
        int sum, carry = 0;

        while (is_there_empty_ptr(lists) || carry) {
            sum = carry;
            calcSum(lists, sum);

            checking->next = new ListNode(sum % 10);
            checking = checking->next;
            carry = sum / 10;
        }
        
        return dummy.next;
    }

    bool is_there_empty_ptr(vector<ListNode*>& lists) {
        int len = lists.size();
        for (int i=0; i<len; i++){
            if (lists.at(i)) {
                return true;
            }
        }
        return false;
    }

    void calcSum(vector<ListNode*>& lists, int& sum) {
        int len = lists.size();
        for (int i=0; i<len; i++){
            if (lists.at(i)) {
                sum += lists.at(i)->val;
                lists.at(i) = lists.at(i)->next;
            }
        }
    }
    

};
```

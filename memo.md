# Step 1
- 問題となるのは繰り上がりの処理
- 繰り上がりが生じる度に都度処理する方針で書いたのが以下のコード．
  - dummyhead`sol`を用意
  - `l1`,`l2`は各桁を指す．桁が存在する間，
    - `l1`,`l2`（存在すれば）の値，及び繰り上がりを足し算．
    - 足し算の結果が
      - 10を超えていなければ，足し算の結果を出力リストの要素に格納
      - 10を超えていれば10引いた値を出力リストの要素に格納し，繰り上がりとして出力リストの次の要素の値を1にセット
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode sol;
        ListNode *digit = &sol;
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

        return sol.next;
    }
};
```
- 短縮できそうな箇所を探す．
  - 最初に`digit->next`用のListNodeを作ってるけど，最後に`sum`が確定してから作れば良い．
  - `l1`,`l2`の有無でいちいち分岐（完全場合分け）させてるけど，「あれば足す」で書けばもっと短い
  - 最後に`sum`と10の大小で分岐させてるけど，繰り上がりを格納する変数`carry`を導入して最初に`sum = carry`とすれば分岐の必要がない．
- ということで出来上がったのが以下のコード
```c++
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode sol;
        ListNode *digit = &sol;
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

        return sol.next;
    }
};
```
- 一旦繰り上がりを無視して足し算して，最後に整形するのが以下のコード
# Step 2

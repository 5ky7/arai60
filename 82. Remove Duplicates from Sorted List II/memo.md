# Step1
- とりあえず書いたのが次のコード
  - 出力を構築する用のポインタ`sol`と重複するかチェックするためのポインタ`node`を用意
  - 
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
# Step2

# Step3

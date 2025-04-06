# Step 1
- まさにスタックの出番ですね，という感想
```c++
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        stack<int> st;
        for (; head; head = head->next) {
            st.push(head->val);
        }

        ListNode dummy;
        ListNode* tail = &dummy;
        while (!st.empty()) {
            tail->next = new ListNode(st.top());
            st.pop();
            tail = tail->next;
        }
        
        return dummy.next;
    }
};
```
# Step 2

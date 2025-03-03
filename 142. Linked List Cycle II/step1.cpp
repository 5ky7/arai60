/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        set<ListNode*> S;
        ListNode *p = head;
        while (p != nullptr && !S.count(p)) {
            S.insert(p);
            p = p->next;
        }
        if (p == nullptr) return NULL;
        return p;
    }
};

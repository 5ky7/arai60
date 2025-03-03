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
        unordered_set<ListNode*> S;
        ListNode *p = head;
        while (true) {
            if(p == nullptr) return NULL;
            else if (S.count(p)) return p;

            S.insert(p);
            p = p->next;
        }
    }
};

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
        while (true) {
            if (p == nullptr) return NULL; // 末端に到達したら終了
            else if(S.count(p)) break; // すでにpが含まれていたら(i.e. ループ発見)while終了

            S.insert(p);
            p = p->next;
        }

        return p;
    }
};

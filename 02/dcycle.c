struct ListNode* detectCycle(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    bool cycle = false;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            cycle = true;
            break;
        }
    }
    if (cycle) {
        struct ListNode* tip = head;
        while (tip != slow) {
            slow = slow->next;
            tip = tip->next;
        }
        if (tip == slow) {
                return tip;
            }
    }
    return NULL;
}

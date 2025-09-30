#include <stdio.h>
typedef struct linknode{
    int val;
    struct linknode* next;
}Node;

Node* rotateRight(Node* head, int k) {
    // 边界条件
    if (head == NULL || head->next == NULL) return head;

    // 遍历链表，求链表的长度。
    Node* curr = head;
    int n = 1;
    while (curr->next != NULL) {
        curr = curr->next;
        n++;
    } // curr->next == NULL 

    k = k % n;
    if (k == 0) return head;

    curr->next = head; // 构成了循环链表

    // 查找索引为n-k-1的结点
    curr = head;
    for (int i = 0; i < n - k - 1; i++) {
        curr = curr->next;
    } // i == n-k-1

    Node* ret = curr->next;
    curr->next = NULL;

    return ret;
}

int main()
{
    Node* head=NULL;
    for(int i=0;i<6;i++){
        Node* node=NULL;
        scanf("%d",node->val);
        node->next=head;
        head=node;
    }
    int k;
    printf(" k= ");
    scanf("%d",&k);
    rotateRight(head,k);
    while(head->next!=NULL){
        printf("%d-->",head->val);
        head=head->next;
    }
    printf("%d",head->val);
    return 0;
}


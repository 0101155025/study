#include <iostream>
using namespace std;
typedef struct ListNode
{
    int val;
    struct ListNode* next;
} ListNode;
ListNode *InitList(int val)
{
    ListNode *node = new ListNode();
    node->val = val;
    node->next = nullptr;
    return node;
}
ListNode* headInsert(ListNode *L, int val)
{
    ListNode *note = new ListNode();
    note->val = val;
    note->next = L->next;
    L->next = note;
    return note;
}
ListNode *traversal(ListNode *L, int pos)
{
    if (L == nullptr || pos < 1) return L;
    ListNode *p = L;
    while (p && pos)
    {
        p = p->next;
        pos--;
    }
    return p;
}
int countLength(ListNode *head)
{
    if (head == nullptr) return 0;
    int cnt = 0;
    ListNode *p = head->next;
    while (p)
    {
        p = p->next;
        cnt++;
    }
    return cnt;
}
ListNode *tailInsert(ListNode *head,int val)
{
    if (head == nullptr) return nullptr;
    ListNode *tail = head;
    while (tail->next != nullptr) tail = tail->next;
    ListNode *note = new ListNode();
    note->val = val;
    note->next = nullptr;
    tail->next = note;
    return note;
}
void deleteList(ListNode *L)
{
    ListNode *p = L->next;
    while (p)
    {
        ListNode *temp = p;
        p = p->next;
        delete temp;
    }
    delete L;
}
void display(ListNode* L)
{
    for (auto p = L;p!=nullptr;p=p->next)
    {
        cout << p->val << " ";
    }
    cout << endl;
}
int insert(ListNode* head,int val,int pos)
{
    ListNode *p = new ListNode();
    while (p && pos - 1)
    {
        p = p->next;
        pos--;
    }
    ListNode *q = new ListNode();
    p->next = q;
    q->val = val;
    q->next = p->next;
    return val;
}
int main() {
    ListNode *head = InitList(0);
    headInsert(head, 1);
    tailInsert(head,2);
    display(head->next);
    deleteList(head);
    head = nullptr;
    return 0;
}
#include <iostream>
using namespace std;

struct ListNode
{
    int val;
    ListNode* next;
    ListNode()
    {
        val = 0;
        next = nullptr;
    }
};

ListNode* headInsert(ListNode *L, int val)
{
    if (L == nullptr) return nullptr;
    ListNode *node = new ListNode();
    node->val = val;
    node->next = L->next;
    L->next = node;
    return node;
}

ListNode *tailInsert(ListNode *head,int val)
{
    if (head == nullptr) return nullptr;
    ListNode *tail = head;
    while (tail->next != nullptr) tail = tail->next;
    ListNode *node = new ListNode();
    node->val = val;
    node->next = nullptr;
    tail->next = node;
    return node;
}

int insert(ListNode* head,int val,int pos)
{
    if (head == nullptr || pos < 1) return -1;
    ListNode *p = head;
    for (int i = 1;i < pos;i++)
    {
        if (p == nullptr) return -1;
        p = p->next;
    }
    ListNode *q = new ListNode();
    q->val = val;
    q->next = p->next;
    p->next = q;
    return val;
}

ListNode *traversal(ListNode *L, int pos)
{
    if (L == nullptr || pos < 1) return nullptr;
    const ListNode *p = L;
    while (p && pos--) p = p->next;
    return const_cast<ListNode*>(p);
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

void  reverseList(ListNode *head)
{
    if (head == nullptr || head->next == nullptr) return;
    ListNode *prev = nullptr;
    ListNode *curr = head->next;
    ListNode *next;
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head->next = prev;
}

void deleteList(ListNode *L)
{
    if (L == nullptr) return;
    ListNode *p = L->next;
    while (p)
    {
        ListNode *temp = p;
        p = p->next;
        delete temp;
    }
    delete L;
}

void deletePos(ListNode *head,int pos)
{
    if (head == nullptr || head->next == nullptr) return;
    if (pos < 1)
    {
        cerr << "Invalid pos" << endl;
        return;
    }
    ListNode *p = head;
    for (int i = 1;i < pos;i++)
    {
        if (p->next == nullptr)
        {
            cerr << "Invalid pos" << endl;
            return;
        }
        p = p->next;
    }
    ListNode *temp = p->next;
    if (temp == nullptr || temp->next == nullptr) cerr << "Invalid pos" << endl;
    p->next = temp->next;
    delete temp;
}

void display(ListNode* L)
{
    if (L == nullptr) return;
    for (auto p = L->next;p != nullptr;p = p->next)
        cout << p->val << " ";
    cout << endl;
}

int main() {
    ListNode *head = new ListNode();
    headInsert(head, 1);
    tailInsert(head,2);
    insert(head,3,2);
    cout << "the content of the list:" << endl;
    display(head);
    cout << "the length of the list:" << countLength(head) << endl;
    deleteList(head);
    head = nullptr;
    return 0;
}
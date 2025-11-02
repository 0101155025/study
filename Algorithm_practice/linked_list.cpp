#include <iostream>
#include <cmath>
using namespace std;

typedef int ElemType;

struct ListNode
{
    ElemType val;
    ListNode* next;
    ListNode()
    {
        val = 0;
        next = nullptr;
    }
};

ListNode* headInsert(ListNode *L, ElemType val)
{
    if (L == nullptr) return nullptr;
    ListNode *node = new ListNode();
    node->val = val;
    node->next = L->next;
    L->next = node;
    return node;
}

ListNode *tailInsert(ListNode *head,ElemType val)
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

ElemType insert(ListNode* head,ElemType val,int pos)
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

int countLength(const ListNode *head)
{
    if (head == nullptr) return 0;
    int cnt = 0;
    const ListNode *p = head->next;
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
    if (p->next == nullptr)
    {
        cerr << "Invalid pos" << endl;
        return;
    }
    ListNode *temp = p->next;
    p->next = temp->next;
    delete temp;
}

void display(const ListNode* L)
{
    if (L == nullptr) return;
    for (const ListNode *p = L->next;p != nullptr;p = p->next)
        cout << p->val << " ";
    cout << endl;
}

ElemType countDown1(const ListNode *head,int repos)
{
    if (head ==  nullptr || head->next == nullptr)
    {
        cerr << "The list is empty" << endl;
        return -1;
    }
    int len = countLength(head);
    if (repos > len || repos < 1)
    {
        cerr << "Invalid repos" << endl;
        return -1;
    }
    ListNode *p = head->next;
    for (int i = 0;i < len - repos;i++)
        p = p->next;
    return p->val;
}

ElemType countDown2(const ListNode *head,int repos)
{
    if (head ==  nullptr || repos < 1)
    {
        cerr << "Invalid repos" << endl;
        return -1;
    }
    ListNode *fast = head->next;
    ListNode *slow = head->next;
    for (int i = 0;i < repos;i++)
    {
        if (fast == nullptr)
        {
            cerr << "Invalid repos" << endl;
            return -1;
        }
        fast = fast->next;
    }
    while (fast)
    {
        fast = fast->next;
        slow = slow->next;
    }
    return slow->val;
}

const ListNode *sameSuffix(const ListNode *head1,const ListNode *head2)
{
    if (head1 == nullptr || head2 == nullptr || head1->next == nullptr || head2->next == nullptr)
    {
        cerr << "One of the list is empty" << endl;
        return nullptr;
    }
    int len1 = countLength(head1);
    int len2 = countLength(head2);
    int diff = abs(len1 - len2);
    const ListNode *fast = (len1 > len2) ? head1 : head2;
    const ListNode *slow = (len1 > len2) ? head2 : head1;
    for (int i = 0;i < diff;i++)
        fast = fast->next;
    while (fast != slow && fast != nullptr && slow != nullptr)
    {
        fast = fast->next;
        slow = slow->next;
    }
    return (fast == slow) ? fast : nullptr;
}

void removeSameNode(ListNode *head,int n)
{
    int *p = new int[n + 1];
    for (int i = 0;i < n + 1;i++)
        p[i] = 0;
    ListNode *curr = head->next;
    while (curr->next)
    {
        int num = fabs(curr->next->val);
        if (p[num] == 0)
        {
            p[num]++;
            curr = curr->next;
        }
        else
        {
            ListNode *temp = curr->next;
            curr->next = temp->next;
            delete temp;
        }
    }
    delete[] p;
}

void reList(ListNode *head)
{
    if (head == nullptr || head->next == nullptr)
    {
        cerr << "The list is empty" << endl;
        return;
    }
    ListNode *fast = head->next;
    ListNode *slow = head;
    while (fast && fast->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    ListNode *first = nullptr;
    ListNode *second = slow->next;
    slow->next = nullptr;
    ListNode *third = nullptr;
    while (second)
    {
        third = second->next;
        second->next = first;
        first = second;
        second = third;
    }
    ListNode *p1 = head;
    ListNode *q1 = first;
    ListNode *p2,*q2;
    while (p1 && q1)
    {
        p2 = p1->next;
        q2 = q1->next;
        p1->next = q1;
        q1->next = p2;
        p1 = p2;
        q1 = q2;
    }
    if (q1 != nullptr) p1->next = q1;
}

bool isCycle(ListNode *head)
{
    ListNode *fast = head->next;
    ListNode *slow = head->next;
    while (fast && fast->next)
    {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow) return true;
    }
    return false;
}

int main() {
    ListNode *head = new ListNode();
    headInsert(head, 1);
    tailInsert(head,2);

    // 显示链表内容
    cout << "the content of the list:" << endl;
    display(head);

    // 测试插入操作
    insert(head,3,2);
    cout << "after insert:" << endl;
    display(head);
    cout << "the length of the list:" << countLength(head) << endl;

    // 测试反转链表
    reverseList(head);
    cout << "after reversing the list:" << endl;
    display(head);

    // 测试删除操作
    deletePos(head,2);
    cout << "after delete:" << endl;
    display(head);
    cout << "the length of the list:" << countLength(head) << endl;

    // 测试倒数第k个节点
    cout << "test1 : the 1st node from the end is:" << countDown1(head,1) << endl;
    cout << "test2 : the 1st node from the end is:" << countDown2(head,1) << endl;

    // 清理第一个链表
    deleteList(head);
    head = nullptr;

    // 测试两个链表的公共后缀
    ListNode *head1 = new ListNode();
    tailInsert(head1,1);
    ListNode *common = tailInsert(head1,2);
    tailInsert(common,3);
    tailInsert(head1,4);
    ListNode *head2 = new ListNode();
    tailInsert(head2,5);
    tailInsert(head2,6);
    ListNode *p = head2;
    while (p->next != nullptr)
        p = p->next;
    p->next = common;
    cout << "List1 content:" << endl;
    display(head1);
    cout << "List2 content:" << endl;
    display(head2);
    const ListNode *commonNode = sameSuffix(head1,head2);
    if (commonNode != nullptr)
        cout << "the common suffix of the two lists is:" << commonNode->val << endl;
    else cout << "No common suffix found" << endl;

    //删除链表
    p->next = nullptr;
    deleteList(head1);
    deleteList(head2);
    head1 = nullptr;
    head2 = nullptr;

    ListNode *head3 = new ListNode();
    tailInsert(head3,1);
    tailInsert(head3,2);
    tailInsert(head3,3);
    tailInsert(head3,4);
    tailInsert(head3,5);
    cout << "List3 content:" << endl;
    display(head3);
    reList(head3);
    cout << "after reversing the list:" << endl;
    display(head3);
    deleteList(head3);
    head3 = nullptr;

    // 测试是否有环
    ListNode *head4 = new ListNode();
    tailInsert(head4,1);
    tailInsert(head4,2);
    tailInsert(head4,3);
    ListNode *temp = new ListNode();
    tailInsert(head4,4);
    tailInsert(head4,5);

    cout << "is the list cycle:" << isCycle(head4) << endl;
    return 0;
}
#include <iostream>
#include <cmath>
using namespace std;

typedef int ElemType;
/*
 * 单链表的定义
 */
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

/*头插法
 * L: 单链表的头指针
 * val: 要插入的元素值
 * 返回值: 插入的节点
 */

ListNode* headInsert(ListNode *L, ElemType val);

/*尾插法
 * head: 单链表的头指针
 * val: 要插入的元素值
 * 返回值: 插入的节点
 */

ListNode *tailInsert(ListNode *head,ElemType val);

/*在指定位置插入元素
 * head: 单链表的头指针
 * val: 要插入的元素值
 * pos: 要插入的位置
 * 返回值: 插入的元素值
 */

ElemType insert(ListNode* head,ElemType val,int pos);

/*
 * 遍历数组
 * head: 单链表的头指针
 * pos: 要遍历的位置
 * 返回值: 遍历到的元素值
 */

ListNode *traversal(ListNode *L, int pos);

/*
 * 统计单链表的长度
 * head: 单链表的头指针
 * 返回值: 单链表的长度
 */

int countLength(const ListNode *head);

/*
 * 反转单链表
 * head: 单链表的头指针
 */

void  reverseList(ListNode *head);

/*
 * 删除单链表
 * head: 单链表的头指针
 */

void deleteList(ListNode *L);

/*
 * 删除单链表指定位置的元素
 * head: 单链表的头指针
 * pos: 要删除的位置
 */

void deletePos(ListNode *head,int pos);

/*
 * 打印单链表
 * head: 单链表的头指针
 */

void display(const ListNode* L);

/*
 * 从后往前数第pos个元素(遍历法)
 * head: 单链表的头指针
 * repos: 要数的位置
 * 返回值: 从后往前数第pos个元素的值
 */

ElemType countDown1(const ListNode *head,int repos);

/*
 * 从后往前数第pos个元素(双指针法)
 * head: 单链表的头指针
 * repos: 要数的位置
 * 返回值: 从后往前数第pos个元素的值
 */

ElemType countDown2(const ListNode *head,int repos);

/*
 * 找到两个单链表的公共后缀节点(双指针法)
 * head1: 第一个单链表的头指针
 * head2: 第二个单链表的头指针
 * 返回值: 公共后缀节点的指针
 */

const ListNode *sameSuffix(const ListNode *head1,const ListNode *head2);

/*
 * 删除单链表中值相同的节点
 * head: 单链表的头指针
 * n: 链表长度,同时也是链表中的最大值
 */

void removeSameNode(ListNode *head,int n);

/*
 * 重新排列单链表(将链表的后半部分插入到前半部分中)
 * head: 单链表的头指针
 */

void reList(ListNode *head);

/*
 * 判断单链表是否有环(双指针法)
 * head: 单链表的头指针
 * 返回值: 如果有环则返回true,否则返回false
 */

bool isCycle(ListNode *head);

/*
 * 找到单链表中环的入口节点(双指针法)
 * head: 单链表的头指针
 * 返回值: 环的入口节点的指针
 */

ListNode *cycleHead(ListNode *head);

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
    ListNode *temp = tailInsert(head4,3);
    tailInsert(head4,4);
    ListNode *tail = tailInsert(head4,5);
    tail->next = temp;

    cout << "is the list cycle:" << isCycle(head4) << endl;
    ListNode *pp = head4->next;
    int count = 0;
    while (p != nullptr && count  < 10)
    {
        cout << pp->val << endl;
        pp = pp->next;
        count++;
    }
    cout << endl;
    // 测试环的入口节点
    ListNode *entry = cycleHead(head4);
    if (entry != nullptr)
        cout << "the cycle head is:" << entry->val << endl;
    else cout << "No cycle head found" << endl;

    tail->next = nullptr;
    deleteList(head4);
    head4 = nullptr;
    if (temp != nullptr)
    {
        delete temp;
        temp = nullptr;
    }

    return 0;
}

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
    if (head ==  nullptr || head->next == nullptr || repos < 1)
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
    if (p1 != nullptr && q1 != nullptr) p1->next = q1;
}

bool isCycle(ListNode *head)
{
    if (head == nullptr || head->next == nullptr)
    {
        cerr << "The list is empty" << endl;
        return false;
    }
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

ListNode *cycleHead(ListNode *head)
{
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
    {
        cerr << "The list has no cycle" << endl;
        return nullptr;
    }
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast && fast->next)
    {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow)
        {
            int cnt = 1;
            ListNode *temp = fast->next;
            while (temp->next != slow)
            {
                temp = temp->next;
                cnt++;
            }
            fast = head;
            slow = head;
            for (int i = 0;i < cnt;i++) fast = fast->next;
            while (fast != slow)
            {
                fast = fast->next;
                slow = slow->next;
            }
            return fast;
        }
    }
    return nullptr;
}
#include <iostream>
using namespace std;

typedef int DoubleLinkType;

// 双链表节点
struct Node {
    DoubleLinkType val;
    Node* prev, *next;
    Node()
    {
        prev = nullptr;
        next = nullptr;
        val = 0;
    }
    Node(DoubleLinkType x)
    {
        prev = nullptr;
        next = nullptr;
        val = x;
    }
};

/*
 * 头插法
 * @param head 头节点
 * @param x 要插入的元素值
 * @return 插入的节点
 */

Node* headInsert(Node* head, DoubleLinkType x);

/**
 * 尾插法
 * @param head 头节点
 * @param x 要插入的元素值
 * @return 插入的节点
 */

Node *tailInsert(Node* head, DoubleLinkType x);

/**
 * 任意位置插入
 * @param head 头节点
 * @param pos 要插入的位置
 * @param x 要插入的元素值
 * @return 插入的节点
 */

Node *posInsert(Node* head, int pos, DoubleLinkType x);

/**
 * 删除任意位置的元素
 * @param head 头节点
 * @param pos 要删除的位置
 * @return 删除的元素值
 */

DoubleLinkType deletePos(Node* head, int pos);

/**
 * 清空双链表
 * @param head 头节点
 */

void clearList(Node* head);

/**
 * 显示双链表元素
 * @param head 头节点
 */

void display(Node* head);

/**
 * 获取双链表长度
 * @param head 头节点
 * @return 双链表长度
 */

int getLength(Node* head);

int main()
{
    Node* head = new Node();
    // 头插法
    cout << "headInsert:" << endl;
    headInsert(head,10);
    headInsert(head,20);
    headInsert(head,30);
    display(head);

    // 尾插法
    cout << "tailInsert:" << endl;
    tailInsert(head,40);
    tailInsert(head,50);
    tailInsert(head,60);
    display(head);

    // 任意位置插入
    cout << "posInsert:" << endl;
    posInsert(head,3,100);
    posInsert(head,5,200);
    posInsert(head,7,300);
    display(head);

    // 获取双链表长度
    cout << "getLength:" << endl << getLength(head) << endl;

    // 删除任意位置的元素
    cout << "deletePos:" << endl;
    deletePos(head,3);
    deletePos(head,4);
    deletePos(head,5);
    display(head);

    clearList(head);
    delete head;
    head = nullptr;
    return 0;
}

Node* headInsert(Node* head, DoubleLinkType x)
{
    if (head == nullptr)
    {
        cerr << "List is empty" << endl;
        return nullptr;
    }
    Node* temp = new Node(x);
    temp->prev = head;
    temp->next = head->next;
    if (head->next != nullptr) head->next->prev = temp;
    head->next = temp;
    return temp;
}

Node *tailInsert(Node* head, DoubleLinkType x)
{
    if (head == nullptr)
    {
        cerr << "List is empty" << endl;
        return nullptr;
    }
    Node* temp = new Node(x);
    while (head->next != nullptr) head = head->next;
    head->next = temp;
    temp->prev = head;
    return temp;
}

Node *posInsert(Node* head, int pos, DoubleLinkType x)
{
    if (head == nullptr)
    {
        cerr << "List is empty" << endl;
        return nullptr;
    }
    Node *temp = head;
    while (pos-- > 1 && temp != nullptr) temp = temp->next;
    if (temp == nullptr)
    {
        cerr << "Position is out of range" << endl;
        return nullptr;
    }
    Node *newNode = new Node(x);
    newNode->prev = temp;
    newNode->next = temp->next;
    if (temp->next != nullptr) temp->next->prev = newNode;
    temp->next = newNode;
    return newNode;
}

DoubleLinkType deletePos(Node* head, int pos)
{
    if (head == nullptr)
    {
        cerr << "List is empty" << endl;
        return 0;
    }
    Node *temp = head;
    while (pos-- > 1 && temp != nullptr) temp = temp->next;
    if (temp == nullptr || temp->next == nullptr)
    {
        cerr << "Position is out of range" << endl;
        return 0;
    }
    Node *delNode = temp->next;
    temp->next = delNode->next;
    if (delNode->next != nullptr) delNode->next->prev = temp;
    DoubleLinkType delVal = delNode->val;
    delete delNode;
    return delVal;
}

void clearList(Node* head)
{
    if (head == nullptr) return;
    Node *curr = head->next;
    while (curr != nullptr)
    {
        Node *next = curr->next;
        delete curr;
        curr = next;
    }
    head = nullptr;
}

void display(Node* head)
{
    if (head == nullptr || head->next == nullptr)
    {
        cerr << "List is empty" << endl;
        return;
    }
    Node *temp = head->next;
    while (temp != nullptr)
    {
        cout << temp->val << " ";
        temp = temp->next;
    }
    cout << endl;
}

int getLength(Node* head)
{
    if (head == nullptr || head->next == nullptr) return 0;
    int len = 0;
    Node *temp = head->next;
    while (temp != nullptr)
    {
        len++;
        temp = temp->next;
    }
    return len;
}

//
// Created by Fly.
//
#include <iostream>
using namespace std;

#define CHAR 1
#include "Algorithm_practice.h"

char str[] = "ABDH##I##EJ###CF##G##";
int inx = 0;

typedef struct ThreadNode
{
    ElemType data;
    ThreadNode *lchild;
    ThreadNode *rchild;
    int ltag;
    int rtag;
}ThreadNode;

typedef ThreadNode *ThreadTree;
ThreadTree pre = nullptr;

//创建树
void createTree(ThreadTree *T);

//中序线索化
void inOrderThreading(ThreadTree *head,ThreadTree T);

//具体线索化
void threading(ThreadTree T);

// 使用线索进行中序遍历
void inOrder(ThreadTree T);

int main()
{
    ThreadTree head = nullptr;
    ThreadTree T = nullptr;
    // 创建树
    createTree(&T);

    // 线索化
    inOrderThreading(&head, T);
    // 基于线索遍历
    inOrder(head);
    return 0;
}

void createTree(ThreadTree *T)
{
    ElemType ch = str[inx++];
    if (ch == '#')
        *T = nullptr;
    else
    {
        *T = new ThreadNode();
        (*T)->data = ch;
        createTree(&(*T)->lchild);
        if ((*T)->lchild != nullptr)
            (*T)->ltag = 0;
        createTree(&(*T)->rchild);
        if ((*T)->rchild != nullptr)
            (*T)->rtag = 0;
    }
}

void inOrderThreading(ThreadTree *head,ThreadTree T)
{
    *head = new ThreadNode();
    (*head)->ltag = 0;
    (*head)->rtag = 1;
    (*head)->rchild = (*head);
    if (T == nullptr)
        (*head)->lchild = *head;
    else
    {
        (*head)->lchild = T;
        pre = (*head);
        threading(T);
        pre->rchild = *head;
        pre->rtag = 1;
        (*head)->rchild = pre;
    }
}

void threading(ThreadTree T)
{
    if (T != nullptr)
    {
        threading(T->lchild);
        if (T->lchild == nullptr)
        {
            T->ltag = 1;
            T->lchild = pre;
        }
        if (pre->rchild == nullptr)
        {
            pre->rtag = 1;
            pre->rchild = T;
        }
        pre = T;
        threading(T->rchild);
    }
}

void inOrder(ThreadTree T)
{
    ThreadTree curr;
    curr = T->lchild;
    while (curr != T)
    {
        while (curr->ltag == 0)
            curr = curr->lchild;
        cout << curr->data << " ";
        while (curr->rtag == 1 && curr->rchild != T)
        {
            curr = curr->rchild;
            cout << curr->data << " ";
        }
        curr = curr->rchild;
    }
    cout << endl;
}

//
// Created by Fly on 2025/11/10.
//
#include <iostream>
using namespace std;

typedef char ElemType;

typedef struct ThreadNode
{
    ElemType data;
    ThreadNode *lchild;
    ThreadNode *rchild;
    int ltag;
    int rtag;
    ThreadNode()
    {
        lchild = nullptr;
        rchild = nullptr;
        ltag = 0;
        rtag = 0;
        data = 0;
    }
    ThreadNode(ElemType data)
    {
        this->data = data;
        lchild = nullptr;
        rchild = nullptr;
        ltag = 0;
        rtag = 0;
    }
}ThreadNode;

typedef ThreadNode *ThreadTree;

char str[] = "ABDH##I##EJ###CF##G##";
int inx = 0;

ThreadTree prev;

int main()
{
    ThreadTree head;
    ThreadTree T = ThreadTree();
    return 0;
}

//
// Created by Fly on 2025/11/10.
//
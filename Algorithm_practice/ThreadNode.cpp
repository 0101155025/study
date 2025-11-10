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
}ThreadNode;

typedef ThreadNode *ThreadTree;

int main()
{
    return 0;
}

//
// Created by Fly on 2025/11/10.
//
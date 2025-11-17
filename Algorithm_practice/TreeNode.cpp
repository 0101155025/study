#include <iostream>
using namespace std;

#define CHAR 1
#include "Algorithm_practice.h"

typedef struct TreeNode
{
    ElemType data;
    TreeNode *lchild,*rchild;
    TreeNode()
    {
        data = 0;
        lchild = nullptr;
        rchild = nullptr;
    }
    explicit TreeNode(ElemType data)
    {
        this->data = data;
        lchild = nullptr;
        rchild = nullptr;
    }
}TreeNode;

typedef TreeNode *BiTree;

char str[] = "ABDH#K###E##CFI###G#J##";
int idx = 0;

/**
 * @brief 创建二叉树
 *
 * @param T 二叉树指针
 */

void createTree(BiTree *T);

/**
 * @brief 前序遍历二叉树
 *
 * @param T 二叉树指针
 */

void preOrder(BiTree T);

/**
 * @brief 中序遍历二叉树
 *
 * @param T 二叉树指针
 */

void inOrder(BiTree T);

/**
 * @brief 后序遍历二叉树
 *
 * @param T 二叉树指针
 */

void postOrder(BiTree T);

int main()
{
    BiTree T = new TreeNode();
    createTree(&T);

    preOrder(T);
    cout << endl;

    inOrder(T);
    cout << endl;

    postOrder(T);
    cout << endl;

    return 0;
}

void createTree(BiTree *T)
{
    ElemType ch;
    ch = str[idx++];
    if (ch == '#') *T = nullptr;
    else
    {
        *T = new TreeNode(ch);
        createTree(&(*T)->lchild);
        createTree(&(*T)->rchild);
    }
}

void preOrder(BiTree T)
{
    if (T == nullptr) return;
    cout << T->data << " ";
    preOrder(T->lchild);
    preOrder(T->rchild);
}

void inOrder(BiTree T)
{
    if (T == nullptr) return;
    inOrder(T->lchild);
    cout << T->data << " ";
    inOrder(T->rchild);
}

void postOrder(BiTree T)
{
    if (T == nullptr) return;
    postOrder(T->lchild);
    postOrder(T->rchild);
    cout << T->data << " ";
}

//
// Created by Fly.
//
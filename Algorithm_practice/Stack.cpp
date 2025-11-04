#include <iostream>
#include <stack>
using namespace std;

#define MAXSIZE 100

typedef int ElemType;

// 栈的实现
typedef struct Stack
{
    ElemType data[MAXSIZE];
    int top;
    Stack()
    {
        top = -1;
    }
}Stack;

/**
 * @brief 入栈操作
 *
 * @param s 栈指针
 * @param e 入栈元素
 * @return true 入栈成功
 * @return false 入栈失败
 */

bool push(Stack *s,ElemType e);

/**
 * @brief 出栈操作
 *
 * @param s 栈指针
 * @return ElemType 出栈元素
 */

bool pop(Stack *s,ElemType *e);

/**
 * @brief 获取栈顶元素
 *
 * @param s 栈指针
 * @return ElemType 栈顶元素
 */

bool getTop(Stack *s,ElemType &e);

/**
 * @brief 销毁栈
 *
 * @param s 栈指针
 */

void DestroyStack(Stack *s)
{
    if (s == nullptr)
        return;
    s->top = -1;
}

/**
 * @brief 显示栈元素
 *
 * @param s 栈指针
 */

void display(Stack *s);

int main()
{
    Stack *s = new Stack();
    // 入栈操作
    cout << "push 1,2,3 to stack:" << endl;
    push(s, 1);
    push(s, 2);
    push(s, 3);
    display(s);

    // 出栈操作
    cout << "pop top element:" << endl;
    ElemType e;
    pop(s,&e);
    display(s);

    // 获取栈顶元素
    if (getTop(s,e))
        cout << "top element is: " << e << endl;

    delete s;
    return 0;
}

bool push(Stack *s,ElemType e)
{
    if (s->top == MAXSIZE - 1)
    {
        cerr << "Stack Overflow" << endl;
        return false;
    }
    s->top++;
    s->data[s->top] = e;
    return true;
}

bool pop(Stack *s,ElemType *e)
{
    if (s == nullptr || s->top == -1)
    {
        cerr << "Stack is empty" << endl;
        return false;
    }
    *e = s->data[s->top];
    s->top--;
    return true;
}

bool getTop(Stack *s,ElemType &e)
{
    if (s == nullptr || s->top == -1)
    {
        cerr << "Stack is empty" << endl;
        return false;
    }
    e = s->data[s->top];
    return true;
}

void display(Stack *s)
{
    if (s == nullptr || s->top == -1)
    {
        cerr << "Stack is empty" << endl;
        return;
    }
    cout << "stack elements are: ";
    for (int i = s->top; i >= 0; i--)
        cout << s->data[i] << " ";
    cout << endl;
}
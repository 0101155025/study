#include <iostream>
using namespace std;

#define MAXSIZE 100

typedef int ElemType;

// 队列的实现
typedef struct Queue
{
    ElemType data[MAXSIZE];
    int front,rear;
    Queue()
    {
        front = 0;
        rear = 0;
    }
}Queue;

/**
 * @brief 入队操作
 *
 * @param q 队列指针
 * @param e 入队元素
 * @return true 入队成功
 * @return false 入队失败
 */

bool eQueue(Queue *q,ElemType e);

/**
 * @brief 出队操作
 *
 * @param s 队列指针
 * @return ElemType 出队元素
 */

ElemType deQueue(Queue *s);

/**
 * @brief 判断队列是否为空
 *
 * @param s 队列指针
 * @return true 队列为空
 * @return false 队列不为空
 */

bool isEmpty(Queue *q);

/**
 * @brief 判断队列是否已满
 *
 * @param q 队列指针
 * @return true 队列已满
 * @return false 队列未满
 */

bool isFull(Queue *q);

/**
 * @brief 获取队列头元素
 *
 * @param q 队列指针
 * @return ElemType 队列头元素
 */

ElemType getTop(Queue *q);

int main()
{
    Queue q;
    eQueue(&q,1);
    eQueue(&q,2);
    eQueue(&q,3);
    cout << "the top of the queue is: " << getTop(&q) << endl;
    deQueue(&q);
    cout << "after deQueue,the top of the queue is: " << getTop(&q) << endl;
    return 0;
}

bool eQueue(Queue *q,ElemType e)
{
    if (q == nullptr)
    {
        cerr<<"the queue does not exist"<<endl;
        return false;
    }
    if (q->rear >= MAXSIZE)
    {
        if (q->front == 0)
        {
            cerr<<"the queue is empty"<<endl;
            return false;
        }
        else
        {
            for (int i = q->front; i < q->rear; i++) q->data[i - q->front] = q->data[i];
            q->rear -= q->front;
            q->front = 0;
        }
    }
    q->data[q->rear] = e;
    q->rear++;
    return true;
}

ElemType deQueue(Queue *q)
{
    if (q == nullptr)
    {
        cerr<<"the queue does not exist"<<endl;
        return -1;
    }
    if (q->front == q->rear)
    {
        cerr<<"the queue is empty"<<endl;
        return -1;
    }
    ElemType e = q->data[q->front];
    q->front++;
    return e;
}


bool isEmpty(Queue *s)
{
    if (s == nullptr)
    {
        cerr<<"the queue does not exist"<<endl;
        return false;
    }
    if (s->front == s->rear)
        return true;
    else
        return false;
}

bool isFull(Queue *q)
{
    if (q == nullptr)
    {
        cerr<<"the queue does not exist"<<endl;
        return false;
    }
    if (q->rear == MAXSIZE && q->front == 0)
        return true;
    else
        return false;
}

ElemType getTop(Queue *q)
{
    if (q == nullptr)
    {
        cerr<<"the queue does not exist"<<endl;
        return -1;
    }
    if (q->front == q->rear)
    {
        cerr<<"the queue is empty"<<endl;
        return -1;
    }
    return q->data[q->front];
}
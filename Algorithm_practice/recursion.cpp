#include <iostream>
using namespace std;

int sum(int n);

int fib1(int n);

int fib2(int n);

int main ()
{
    int n;
    cout << "function sum test:" << endl;
    scanf("%d",&n);
    printf("%d\n",sum(n));

    cout << "function fib1 test:" << endl;
    printf("%d\n",fib1(n));

    cout << "function fib2 test:" << endl;
    printf("%d\n",fib2(n));
    return 0;
}

int sum(int n)
{
    if (n <= 1) return 1;
    else return sum(n - 1) + n;
}

int fib1(int n)
{
    if (n == 1 || n == 2) return 1;
    if (n < 1)
    {
        cerr<<"n must be greater than 1"<<endl;
        return -1;
    }
    else return fib1(n - 1) + fib1(n - 2);
}

int fib2(int n)
{
    if (n == 1 || n == 2) return 1;
    if (n < 1)
    {
        cerr<<"n must be greater than 1"<<endl;
        return -1;
    }
    int a = 1,b = 1,c;
    for (int i = 2;i <= n;i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

//
// Created by Fly.
//
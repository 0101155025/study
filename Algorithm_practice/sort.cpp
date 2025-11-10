#include <iostream>
using namespace std;

/**
 * @brief 冒泡排序
 *
 * @param a 待排序数组
 * @param n 数组长度
 */

void bubbleSort(int a[],int n);

/**
 * @brief 快速排序
 *
 * @param a 待排序数组
 * @param l 数组左边界
 * @param r 数组右边界
 */
void quickSort(int a[],int l,int r);

/**
 * @brief 显示数组元素
 *
 * @param a 数组
 * @param n 数组长度
 */
void display(int a[],int n);

int main()
{
    // 冒泡排序
    int a[10] = {1,3,2,4,5,6,7,8,9,10};
    bubbleSort(a,10);
    display(a,10);

    // 快速排序
    int b[10] = {1,3,2,4,5,8,7,6,9,8};
    quickSort(b,0,9);
    display(b,10);
    return 0;
}

void bubbleSort(int a[],int n)
{
    if (n <= 0 || a == nullptr)
    {
        cerr << "the array is empty or n is invalid" << endl;
        return;
    }
    for (int i = 1;i < n;i++)
    {
        for (int j = 0;j < n - i;j++)
        {
            if (a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void quickSort(int a[],int l,int r)
{
    if (l >= r || a == nullptr)
    {
        cerr << "the array is empty or l or r is invalid" << endl;
        return;
    }
    int left = l,right = r,temp = 0;
    if (left <= right)
    {
        temp = a[left];
        while (left != right)
        {
            while (right > left && a[right] > temp)
                right--;
            a[left] = a[right];
            while (left < right && a[left] <= temp)
                left++;
            a[right] = a[left];
        }
        a[left] = temp;
        quickSort(a,l,left - 1);
        quickSort(a,right + 1,r);
    }
}

void display(int a[],int n)
{
    if (n <= 0 || a == nullptr)
    {
        cerr << "the array is empty or n is invalid" << endl;
        return;
    }
    for (int i = 0;i < n;i++)
        cout<<a[i]<<" ";
    cout<<endl;
}

//
// Created by Fly on 2025/11/10.
//
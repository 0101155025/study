#include <iostream>
#include <cmath>
using namespace std;

/*
 * 过完白厄剧情,了解到他的轮回数是完全数1111111111111000000000000,很感兴趣,所以来计算一下
 * 这里采用基于梅森素数的完全数计算方法。
 * 梅森素数是指2^p-1是素数的数，其中p是一个素数。
 * 例如，2^2-1=3是素数，所以2^2-1是梅森素数。
 * 完全数是指所有因子（除了它本身）的和等于它本身的数。
 * 例如，6是完全数，因为它的因子（1,2,3）的和等于6。
 */

// 判断是否为素数
bool isPrime(int p) {
    if (p <= 1) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (int i = 3; i <= sqrt(p); i += 2) {
        if (p % i == 0) return false;
    }
    return true;
}

// 计算2^p-1是否为梅森素数
long long mersennePrime(int p) {
    if (!isPrime(p)) return -1; // 只有当p是素数时，2^p-1才可能是素数
    long long result = 1;
    for (int i = 0; i < p; i++) {
        result *= 2;
    }
    return result - 1;
}

int main() {
    // 只检查前几个可能的梅森素数对应的完全数
    // 已知的前几个梅森素数对应的指数p
    int primes[] = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127};
    int count = sizeof(primes) / sizeof(primes[0]);

    for (int i = 0; i < count; i++) {
        int p = primes[i];
        long long m = mersennePrime(p);
        if (m != -1) { // 确认是梅森素数
            long long perfectNum = (m + 1) / 2 * m;
            cout << perfectNum << endl;
        }
    }
    return 0;
}


//
// Created by MR on 2025/11/19.
//
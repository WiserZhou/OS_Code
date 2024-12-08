#include <stdio.h>
#include "banker.h"

int main()
{
    BankerSystem system;
    int n = 5; // 进程数
    int m = 3; // 资源类型数

    // 初始化系统
    initBankerSystem(&system, n, m);

    // 设置Available资源
    system.Available[0] = 3;
    system.Available[1] = 3;
    system.Available[2] = 2;

    // 设置分配矩阵和请求矩阵
    // 这里需要根据实际情况设置具体的值

    // 运行安全性检查
    if (safetyCheck(&system))
    {
        printf("系统处于安全状态\n");
    }
    else
    {
        printf("系统处于死锁状态\n");
    }

    // 释放资源
    freeBankerSystem(&system);

    return 0;
}
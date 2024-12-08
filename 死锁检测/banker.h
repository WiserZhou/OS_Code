#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>

typedef struct
{
    int n;            // 进程数量
    int m;            // 资源类型数量
    int *Available;   // 可用资源向量
    int **Request;    // 资源请求矩阵
    int **Allocation; // 已分配矩阵
    bool *Finish;     // 完成向量
} BankerSystem;

// 初始化银行家算法系统
void initBankerSystem(BankerSystem *system, int n, int m);

// 安全性检查算法
bool safetyCheck(BankerSystem *system);

// 释放系统资源
void freeBankerSystem(BankerSystem *system);

#endif
#ifndef BANKER_H
#define BANKER_H

typedef struct {
    int n;              // 进程数量
    int m;              // 资源类型数量
    int *Available;     // 可用资源向量
    int **Max;         // 最大需求矩阵
    int **Allocation;  // 已分配矩阵
    int **Need;        // 需求矩阵
    bool *Finish;      // 完成向量
} BankerSystem;

// 初始化银行家算法系统
void initBankerSystem(BankerSystem *system, int n, int m);

// 资源请求算法
bool requestResource(BankerSystem *system, int pid, int *request);

// 安全性检查算法
bool safetyCheck(BankerSystem *system);

#endif 
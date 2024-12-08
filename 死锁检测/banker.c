#include <stdio.h>
#include <stdlib.h>
#include "banker.h"

void initBankerSystem(BankerSystem *system, int n, int m) {
    system->n = n;
    system->m = m;
    
    // 分配内存
    system->Available = (int*)malloc(m * sizeof(int));
    system->Finish = (bool*)malloc(n * sizeof(bool));
    
    system->Request = (int**)malloc(n * sizeof(int*));
    system->Allocation = (int**)malloc(n * sizeof(int*));
    
    for(int i = 0; i < n; i++) {
        system->Request[i] = (int*)malloc(m * sizeof(int));
        system->Allocation[i] = (int*)malloc(m * sizeof(int));
    }
}

bool safetyCheck(BankerSystem *system) {
    int *Work = (int*)malloc(system->m * sizeof(int));
    bool deadlock = false;
    
    // STEP 1: 初始化Work和Finish
    for(int i = 0; i < system->m; i++) {
        Work[i] = system->Available[i];
    }
    
    for(int i = 0; i < system->n; i++) {
        bool hasAllocation = false;
        for(int j = 0; j < system->m; j++) {
            if(system->Allocation[i][j] != 0) {
                hasAllocation = true;
                break;
            }
        }
        system->Finish[i] = !hasAllocation;
    }
    
    // STEP 2 & 3: 查找可以完成的进程
    bool found;
    do {
        found = false;
        for(int i = 0; i < system->n; i++) {
            if(!system->Finish[i]) {
                bool canAllocate = true;
                // 检查是否满足Request <= Work
                for(int j = 0; j < system->m; j++) {
                    if(system->Request[i][j] > Work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                
                if(canAllocate) {
                    // 分配资源
                    for(int j = 0; j < system->m; j++) {
                        Work[j] += system->Allocation[i][j];
                    }
                    system->Finish[i] = true;
                    found = true;
                }
            }
        }
    } while(found);
    
    // STEP 4: 检查是否存在死锁
    for(int i = 0; i < system->n; i++) {
        if(!system->Finish[i]) {
            deadlock = true;
            break;
        }
    }
    
    free(Work);
    return !deadlock;
}

void freeBankerSystem(BankerSystem *system) {
    free(system->Available);
    free(system->Finish);
    
    for(int i = 0; i < system->n; i++) {
        free(system->Request[i]);
        free(system->Allocation[i]);
    }
    
    free(system->Request);
    free(system->Allocation);
}
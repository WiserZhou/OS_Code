/* 引入必要的头文件 */
#include <pthread.h> // 线程相关函数
#include <stdio.h>   // 标准输入输出
#include <stdlib.h>  // 标准库函数

/* 定义要创建的线程数量为5 */
#define NUM_THREADS 5

/* 线程执行的函数 */
void *runner(void *param)
{
    /* 线程执行一些工作... */
    pthread_exit(0); // 线程退出
}

int main(int argc, char *argv[])
{
    /* 定义线程ID数组和线程属性变量 */
    pthread_t tid[NUM_THREADS]; // 存储线程ID
    pthread_attr_t attr;        // 线程属性

    /* 初始化线程属性为默认值 */
    pthread_attr_init(&attr);

    /* 首先查询当前的线程调度范围 */
    int scope;
    if (pthread_attr_getscope(&attr, &scope) != 0)
    {
        fprintf(stderr, "Unable to get scheduling scope\n");
    }
    else
    {
        /* 输出当前的线程调度范围 */
        if (scope == PTHREAD_SCOPE_PROCESS)
            printf("PTHREAD_SCOPE_PROCESS\n"); // 进程级竞争范围
        else if (scope == PTHREAD_SCOPE_SYSTEM)
            printf("PTHREAD_SCOPE_SYSTEM\n"); // 系统级竞争范围
        else
            fprintf(stderr, "Illegal scope value.\n");
    }

    /* 设置线程调度算法为系统级竞争范围(SCS) */
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

    /* 创建NUM_THREADS个线程 */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&tid[i], &attr, runner, NULL);
    }

    /* 等待所有线程执行完成 */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
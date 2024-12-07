/* 引入必要的头文件 */
#include <pthread.h> // 线程相关函数
#include <stdio.h>   // 标准输入输出
#include <stdlib.h>  // 标准库函数

/* 全局变量sum，用于存储计算结果，被两个线程共享 */
int sum;

/* 声明两个线程运行的函数 */
void *runner01(void *param); // 线程1：用于累加计算
void *runner02(void *param); // 线程2：用于累减计算

int main(int argc, char *argv[])
{
    /* 初始化sum为0 */
    sum = 0;
    /* 定义两个线程ID变量 */
    pthread_t tid01;
    pthread_t tid02;
    /* 定义线程属性变量 */
    pthread_attr_t attr;

    /* 检查命令行参数数量是否正确 */
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    /* 检查输入的数字是否为正数 */
    if (atoi(argv[1]) <= 0)
    {
        fprintf(stderr, "%d must be > 0\n", atoi(argv[1]));
        return -1;
    }

    /* 初始化线程属性为默认值 */
    pthread_attr_init(&attr);
    /* 创建两个新线程，分别执行runner01和runner02函数 */
    pthread_create(&tid01, &attr, runner01, argv[1]);
    pthread_create(&tid02, &attr, runner02, argv[1]);
    /* 等待两个线程都执行完成 */
    pthread_join(tid01, NULL);
    pthread_join(tid02, NULL);

    /* 主线程退出 */
    pthread_exit(0);
}

/* 线程1执行的函数：计算从1到输入值的累加和 */
void *runner01(void *param)
{
    /* 将传入的参数转换为整数，作为累加的上限 */
    int i, upper = atoi(param);
    /* 循环累加并打印每一步的结果 */
    for (i = 1; i <= upper; i++)
    {
        sum += i;
        printf("[thread 1] sum = %d\n", sum);
    }
    return NULL;
}

/* 线程2执行的函数：计算从1到输入值的累减和 */
void *runner02(void *param)
{
    /* 将传入的参数转换为整数，作为累减的上限 */
    int i, lower = atoi(param);
    /* 循环累减并打印每一步的结果 */
    for (i = 1; i <= lower; i++)
    {
        sum -= i;
        printf("[thread 2] sum = %d\n", sum);
    }
    return NULL;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* 全局变量sum，用于存储计算结果，被线程共享 */
int sum;
/* 线程运行的函数声明 */
void *runner(void *param);

int main(int argc, char *argv[])
{
    /* tid用于存储线程ID */
    pthread_t tid;
    /* attr用于设置线程属性 */
    pthread_attr_t attr;

    /* 检查命令行参数数量是否正确 */
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }
    /* 检查输入的数字是否为非负数 */
    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    /* 初始化线程属性为默认值 */
    pthread_attr_init(&attr);
    /* 创建新线程，传入命令行参数作为计算上限 */
    pthread_create(&tid, &attr, runner, argv[1]);
    /* 等待线程执行完成 */
    pthread_join(tid, NULL);

    /* 打印计算结果 */
    printf("sum = %d\n", sum);
    return 0;
}

/* 线程执行的函数，用于计算从1到输入值的累加和 */
void *runner(void *param)
{
    /* 将传入的参数转换为整数，作为累加的上限 */
    int i, upper = atoi(param);
    /* 初始化累加和为0 */
    sum = 0;

    /* 循环计算累加和 */
    for (i = 1; i <= upper; i++)
        sum += i;

    /* 线程退出 */
    pthread_exit(0);
}
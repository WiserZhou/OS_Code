/* 引入必要的头文件 */
#include <pthread.h> // 线程相关函数
#include <stdio.h>   // 标准输入输出
#include <unistd.h>  // POSIX操作系统API
#include <sched.h>   // 调度相关函数

// 获取线程调度策略的函数
static int get_thread_policy(pthread_attr_t attr)
{
    int policy;
    // 获取线程的调度策略
    pthread_attr_getschedpolicy(&attr, &policy);

    // 根据不同的调度策略输出对应的信息
    switch (policy)
    {
    case SCHED_FIFO: // 先进先出调度策略
        printf("policy = SCHED_FIFO\n");
        break;
    case SCHED_RR: // 时间片轮转调度策略
        printf("policy = SCHED_RR\n");
        break;
    case SCHED_OTHER: // 默认的分时调度策略
        printf("policy = SCHED_OTHER\n");
        break;
    default: // 未知的调度策略
        printf("policy = UNKNOWN\n");
        break;
    }
    return policy;
}

// 显示指定调度策略下的线程优先级范围
static void show_thread_priority(pthread_attr_t attr, int policy)
{
    // 获取并显示最高优先级
    int priority = sched_get_priority_max(policy);
    printf("max_priority = %d\n", priority);
    // 获取并显示最低优先级
    priority = sched_get_priority_min(policy);
    printf("min_priority = %d\n", priority);
}

// 获取线程的当前优先级
static int get_thread_priority(pthread_attr_t attr)
{
    struct sched_param param;
    // 获取线程的调度参数
    pthread_attr_getschedparam(&attr, &param);
    printf("priority = %d\n", param.sched_priority);
    return param.sched_priority;
}

// 设置线程的调度策略
static void set_thread_policy(pthread_attr_t attr, int policy)
{
    // 设置新的调度策略
    pthread_attr_setschedpolicy(&attr, policy);
    // 显示设置后的调度策略
    get_thread_policy(attr);
}

int main(void)
{
    pthread_attr_t attr;      // 线程属性结构
    struct sched_param sched; // 调度参数结构
    // 初始化线程属性
    int rs = pthread_attr_init(&attr);
    // 获取当前的调度策略
    int policy = get_thread_policy(attr);

    printf("-- show current configuration of priority\n");
    // 显示当前调度策略下的优先级范围
    show_thread_priority(attr, policy);

    printf("-- show SCHED_FIFO of priority\n");
    // 显示FIFO调度策略下的优先级范围
    show_thread_priority(attr, SCHED_FIFO);

    printf("-- show SCHED_RR of priority\n");
    // 显示时间片轮转调度策略下的优先级范围
    show_thread_priority(attr, SCHED_RR);

    printf("-- show priority of current thread\n");
    // 获取当前线程的优先级
    int priority = get_thread_priority(attr);

    printf("SET THREAD POLICY\n");
    printf("set SCHED_FIFO policy\n");
    // 设置为FIFO调度策略
    set_thread_policy(attr, SCHED_FIFO);

    printf("set SCHED_RR policy\n");
    // 设置为时间片轮转调度策略
    set_thread_policy(attr, SCHED_RR);

    printf("restore current policy\n");
    // 恢复原来的调度策略
    set_thread_policy(attr, policy);

    // 销毁线程属性对象
    pthread_attr_destroy(&attr);
    return 0;
}

// policy = SCHED_OTHER
// -- show current configuration of priority
// max_priority = 0
// min_priority = 0
// -- show SCHED_FIFO of priority
// max_priority = 99
// min_priority = 1
// -- show SCHED_RR of priority
// max_priority = 99
// min_priority = 1
// -- show priority of current thread
// priority = 0
// SET THREAD POLICY
// set SCHED_FIFO policy
// policy = SCHED_FIFO
// set SCHED_RR policy
// policy = SCHED_RR
// restore current policy
// policy = SCHED_OTHER
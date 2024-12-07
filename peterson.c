#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 共享变量声明
bool flag[2] = {false, false}; // 进程意图标志
int turn = 0;                  // 轮到谁进入临界区

void process0()
{
    while (true)
    {
        flag[0] = true; // 表示进程0想进入临界区
        turn = 1;       // 让进程1先走

        // 等待直到进程1不想进入或者轮到进程0
        while (flag[1] && turn == 1)
        {
            // 忙等待
        }

        // 临界区
        printf("Process 0 is in critical section\n");
        sleep(1); // 模拟在临界区中的工作

        // 退出临界区
        flag[0] = false;
        printf("Process 0 left critical section\n");
        sleep(1); // 模拟非临界区的工作
    }
}

void process1()
{
    while (true)
    {
        flag[1] = true; // 表示进程1想进入临界区
        turn = 0;       // 让进程0先走

        // 等待直到进程0不想进入或者轮到进程1
        while (flag[0] && turn == 0)
        {
            // 等待
        }

        // 临界区
        printf("Process 1 is in critical section\n");
        sleep(1); // 模拟在临界区中的工作

        // 退出临界区
        flag[1] = false;
        printf("Process 1 left critical section\n");
        sleep(1); // 模拟非临界区的工作
    }
}

int main()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // 子进程运行process1
        process1();
    }
    else
    {
        // 父进程运行process0
        process0();
    }

    return 0;
}
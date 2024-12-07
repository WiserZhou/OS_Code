/* 引入必要的头文件 */
#include <sys/types.h> // 提供基本系统数据类型
#include <stdio.h>     // 标准输入输出
#include <stdlib.h>    // 标准库函数
#include <unistd.h>    // UNIX标准函数定义

/**
 * 字符串输出函数
 * 该函数一次输出一个字符,用于演示竞争条件
 * @param str 要输出的字符串
 */
static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);        /* 设置标准输出为无缓冲模式 */
    for (ptr = str; c = *ptr++;) // 逐个字符输出
        putc(c, stdout);
}

/**
 * 主函数
 * 创建父子进程,演示输出竞争
 */
int main(void)
{
    pid_t pid; // 进程ID

    if ((pid = fork()) < 0)
    {                         // 创建子进程
        perror("fork error"); // fork失败,打印错误信息
    }
    else if (pid == 0)
    { // 子进程执行代码
        charatatime("output from child\n");
    }
    else
    { // 父进程执行代码
        charatatime("output from parent\n");
    }

    exit(0); // 进程退出
}
// output from parenotu
// tput from child
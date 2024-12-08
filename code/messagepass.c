// 引入必要的头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  // IPC通信相关
#include <sys/msg.h>  // 消息队列相关
#include <unistd.h>   // fork()函数
#include <sys/wait.h> // wait()函数

// 定义消息结构体
struct msg
{
    long msg_type;     // 消息类型
    char msg_buf[512]; // 消息内容缓冲区
};

int main()
{
    // 创建消息队列,返回队列ID
    // IPC_PRIVATE表示创建新队列,0666表示读写权限
    int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (qid < 0)
    {
        perror("msgget");
        exit(1);
    }

    // 创建子进程P1
    pid_t p1 = fork();
    if (p1 == 0)
    {
        // P1进程代码块
        struct msg msg1;
        msg1.msg_type = 1;                                   // 设置消息类型为1
        sprintf(msg1.msg_buf, "Message from P1: Hello P2!"); // 构造消息内容
        msgsnd(qid, &msg1, strlen(msg1.msg_buf) + 1, 0);     // 发送消息到队列
        printf("P1 sent: %s\n", msg1.msg_buf);
        exit(0);
    }

    // 创建子进程P2
    pid_t p2 = fork();
    if (p2 == 0)
    {
        // P2进程代码块
        struct msg msg2;
        // 从队列接收类型为1的消息
        msgrcv(qid, &msg2, sizeof(msg2.msg_buf), 1, 0);
        printf("P2 received: %s\n", msg2.msg_buf);
        exit(0);
    }

    // 父进程等待两个子进程结束
    wait(NULL);
    wait(NULL);
    // 删除消息队列
    msgctl(qid, IPC_RMID, NULL);
    return 0;
}

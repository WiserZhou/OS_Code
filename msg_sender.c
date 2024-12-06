#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <strings.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg
{
    long msg_types;
    char msg_buf[511];
};

int main(void)
{
    int qid, pid, len;
    struct msg pmsg;

    // 构造消息内容，包含进程ID
    sprintf(pmsg.msg_buf, "Hello! This is :%d\n", getpid());
    len = strlen(pmsg.msg_buf);

    // 创建消息队列
    if ((qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget failed!");
        exit(1);
    }

    // 发送消息到队列
    if ((msgsnd(qid, &pmsg, len, 0)) < 0)
    {
        perror("msgsnd failed!");
        exit(1);
    }

    printf("Send a message to the queue successfuly : %d\n", qid);
    exit(1);
}
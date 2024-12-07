typedef struct process
{
    // 进程控制块
    struct process *next; // 用于构建等待队列
} process;

typedef struct
{
    int value;
    struct process *list; // 等待进程队列
} semaphore;

void wait(semaphore *S)
{
    S->value--;
    if (S->value < 0)
    {
        // 将当前进程加入S->list
        add_this_process_to_S_list(S);
        block(); // 阻塞当前进程
    }
}

void signal(semaphore *S)
{
    S->value++;
    if (S->value <= 0)
    {
        // 从等待队列中移除一个进程P
        process *P = remove_process_from_list(S);
        wakeup(P); // 唤醒进程P
    }
}

// 辅助函数声明
void add_this_process_to_S_list(semaphore *S);
process *remove_process_from_list(semaphore *S);
void block(void);
void wakeup(process *P);
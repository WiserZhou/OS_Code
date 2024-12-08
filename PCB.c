#include <stdio.h>
#include <stdlib.h>

// 进程控制块(PCB)结构体定义
typedef struct node
{
    int procID;        // 进程ID标识符
    int releaseTime;   // 进程到达/释放时间
    int priority;      // 进程优先级（数值越小优先级越高）
    int cpuTime;       // 进程需要的总CPU运行时间
    int executedTime;  // 进程已经执行的时间
    int state;         // 进程当前状态 (0: 就绪态, 1: 运行态, 2: 等待态)
    struct node *next; // 链表指针，指向下一个进程
} PCB;

PCB *createProcess(int id, int releaseTime, int priority, int cpuTime)
{
    PCB *newProcess = (PCB *)malloc(sizeof(PCB));
    newProcess->procID = id;
    newProcess->releaseTime = releaseTime;
    newProcess->priority = priority;
    newProcess->cpuTime = cpuTime;
    newProcess->executedTime = 0;
    newProcess->state = 0; // 初始为就绪状态
    newProcess->next = NULL;
    return newProcess;
}

void loadTasks(const char *filename, PCB **readyQueue)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开任务文件。\n");
        return;
    }

    int id, releaseTime, priority, cpuTime;
    while (fscanf(file, "%d %d %d %d", &id, &releaseTime, &priority, &cpuTime) != EOF)
    {
        PCB *newProcess = createProcess(id, releaseTime, priority, cpuTime);
        // 将进程插入到就绪队列，按到达时间排序
        if (*readyQueue == NULL)
        {
            *readyQueue = newProcess;
        }
        else
        {
            PCB *temp = *readyQueue;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newProcess;
        }
    }
    fclose(file);
}

// 短作业优先(SJF)调度算法
void SJF(PCB *readyQueue)
{
    PCB *runningProcess = NULL; // 当前运行进程
    int currentTime = 0;        // 当前时间

    while (readyQueue != NULL || runningProcess != NULL)
    {
        // 检查并处理到达的新进程
        while (readyQueue != NULL && readyQueue->releaseTime <= currentTime)
        {
            PCB *newProcess = readyQueue;
            readyQueue = readyQueue->next;
            newProcess->next = NULL;

            // 按照作业长度将新进程插入到适当位置
            if (runningProcess == NULL)
            {
                PCB *temp = runningProcess;
                PCB *prev = NULL;
                // 找到合适的插入位置（按CPU时间升序）
                while (temp != NULL && temp->cpuTime <= newProcess->cpuTime)
                {
                    prev = temp;
                    temp = temp->next;
                }
                if (prev == NULL)
                {
                    newProcess->next = runningProcess;
                    runningProcess = newProcess;
                }
                else
                {
                    newProcess->next = prev->next;
                    prev->next = newProcess;
                }
            }
            else
            {
                // 如果有正在运行的进程，将新进程插入等待队列
                PCB *temp = runningProcess->next;
                PCB *prev = runningProcess;
                while (temp != NULL && temp->cpuTime <= newProcess->cpuTime)
                {
                    prev = temp;
                    temp = temp->next;
                }
                newProcess->next = prev->next;
                prev->next = newProcess;
            }
        }

        // 进程调度和执行
        if (runningProcess != NULL)
        {
            // 输出当前执行状态
            printf("当前时间: %d, 运行进程: P%d\n", currentTime, runningProcess->procID);
            runningProcess->executedTime++;

            // 检查进程是否完成
            if (runningProcess->executedTime == runningProcess->cpuTime)
            {
                printf("进程 P%d 结束。\n", runningProcess->procID);
                PCB *temp = runningProcess;
                runningProcess = runningProcess->next;
                free(temp); // 释放完成的进程
            }

            currentTime++;
        }
        else if (readyQueue != NULL)
        {
            // 无运行进程但有等待进程时，推进时间
            currentTime = readyQueue->releaseTime;
        }
    }
}

void PSJF(PCB *readyQueue)
{
    PCB *runningProcess = NULL;
    int currentTime = 0;

    while (readyQueue != NULL || runningProcess != NULL)
    {
        // 检查是否有新进程到达就绪队列
        while (readyQueue != NULL && readyQueue->releaseTime <= currentTime)
        {
            PCB *newProcess = readyQueue;
            readyQueue = readyQueue->next;
            newProcess->next = NULL;

            // 将新进程插入到就绪队列的正确位置
            PCB *temp = runningProcess;
            PCB *prev = NULL;
            while (temp != NULL && (temp->cpuTime - temp->executedTime) <= (newProcess->cpuTime - newProcess->executedTime))
            {
                prev = temp;
                temp = temp->next;
            }
            if (prev == NULL)
            {
                newProcess->next = runningProcess;
                runningProcess = newProcess;
            }
            else
            {
                newProcess->next = prev->next;
                prev->next = newProcess;
            }

            // 如果新进程比当前运行进程的剩余时间短，则抢占
            if (runningProcess != newProcess &&
                (newProcess->cpuTime - newProcess->executedTime) < (runningProcess->cpuTime - runningProcess->executedTime))
            {
                printf("进程 P%d 被进程 P%d 抢占。\n", runningProcess->procID, newProcess->procID);
                PCB *temp = runningProcess;
                runningProcess = newProcess;
                newProcess = temp;
            }
        }

        if (runningProcess != NULL)
        {
            // 模拟运行
            printf("当前时间: %d, 运行进程: P%d\n", currentTime, runningProcess->procID);
            runningProcess->executedTime++;

            // 如果进程完成运行
            if (runningProcess->executedTime == runningProcess->cpuTime)
            {
                printf("进程 P%d 结束。\n", runningProcess->procID);
                PCB *temp = runningProcess;
                runningProcess = runningProcess->next;
                free(temp);
            }
            else
            {
                // 检查是否需要重新调度
                if (runningProcess->next != NULL &&
                    (runningProcess->cpuTime - runningProcess->executedTime) >
                        (runningProcess->next->cpuTime - runningProcess->next->executedTime))
                {
                    PCB *temp = runningProcess;
                    runningProcess = runningProcess->next;
                    temp->next = runningProcess->next;
                    runningProcess->next = temp;
                    printf("进程 P%d 被进程 P%d 抢占。\n", temp->procID, runningProcess->procID);
                }
            }

            currentTime++;
        }
        else if (readyQueue != NULL)
        {
            // 如果没有进程在运行，但还有进程在等待，时间向前推进到下一个进程的到达时间
            currentTime = readyQueue->releaseTime;
        }
    }
}

void PPrio(PCB *readyQueue)
{
    PCB *runningProcess = NULL;
    PCB *waitingQueue = NULL;
    int currentTime = 0;

    while (readyQueue != NULL || runningProcess != NULL || waitingQueue != NULL)
    {
        // 检查是否有新进程到达就绪队列
        while (readyQueue != NULL && readyQueue->releaseTime <= currentTime)
        {
            PCB *newProcess = readyQueue;
            readyQueue = readyQueue->next;
            newProcess->next = NULL;

            // 将新进程插入到等待队列的正确位置
            if (waitingQueue == NULL || newProcess->priority < waitingQueue->priority)
            {
                newProcess->next = waitingQueue;
                waitingQueue = newProcess;
            }
            else
            {
                PCB *temp = waitingQueue;
                while (temp->next != NULL && temp->next->priority <= newProcess->priority)
                {
                    temp = temp->next;
                }
                newProcess->next = temp->next;
                temp->next = newProcess;
            }
        }

        // 如果没有正在运行的进程，从等待队列中选择优先级最高的进程
        if (runningProcess == NULL && waitingQueue != NULL)
        {
            runningProcess = waitingQueue;
            waitingQueue = waitingQueue->next;
            runningProcess->next = NULL;
        }

        if (runningProcess != NULL)
        {
            // 模拟运行
            printf("P%d:%d, ", runningProcess->procID, currentTime);
            runningProcess->executedTime++;

            // 如果进程完成运行
            if (runningProcess->executedTime == runningProcess->cpuTime)
            {
                printf("\n进程 P%d 结束。\n", runningProcess->procID);
                free(runningProcess);
                runningProcess = NULL;
            }

            currentTime++;
        }
        else if (readyQueue != NULL)
        {
            // 如果没有进程在运行，但还有进程在等待到达，时间向前推进到下一个进程的到达时间
            currentTime = readyQueue->releaseTime;
        }
    }
    printf("\n");
}

void Prio(PCB *readyQueue)
{
    PCB *runningProcess = NULL;
    int currentTime = 0;

    while (readyQueue != NULL || runningProcess != NULL)
    {
        // 检查是否有新进程到达就绪队列
        while (readyQueue != NULL && readyQueue->releaseTime <= currentTime)
        {
            PCB *newProcess = readyQueue;
            readyQueue = readyQueue->next;
            newProcess->next = NULL;

            // 将新进程插入到就绪队列的正确位置
            if (runningProcess == NULL)
            {
                runningProcess = newProcess;
            }
            else
            {
                PCB *temp = runningProcess;
                PCB *prev = NULL;
                while (temp != NULL && temp->priority <= newProcess->priority)
                {
                    prev = temp;
                    temp = temp->next;
                }
                if (prev == NULL)
                {
                    newProcess->next = runningProcess;
                    runningProcess = newProcess;
                    printf("进程 P%d 被进程 P%d 抢占。\n", newProcess->next->procID, newProcess->procID);
                }
                else
                {
                    newProcess->next = prev->next;
                    prev->next = newProcess;
                }
            }
        }

        if (runningProcess != NULL)
        {
            // 模拟运行
            printf("当前时间: %d, 运行进程: P%d\n", currentTime, runningProcess->procID);
            runningProcess->executedTime++;

            // 如果进程完成运行
            if (runningProcess->executedTime == runningProcess->cpuTime)
            {
                printf("进程 P%d 结束。\n", runningProcess->procID);
                PCB *temp = runningProcess;
                runningProcess = runningProcess->next;
                free(temp);
            }

            currentTime++;
        }
        else if (readyQueue != NULL)
        {
            // 如果没有进程在运行，但还有进程在等待，时间向前推进到下一个进程的到达时间
            currentTime = readyQueue->releaseTime;
        }
    }
}

// 添加等待队列
PCB *waitingQueue = NULL;

// 释放进程队列占用的内存
void freeMemory(PCB *queue)
{
    while (queue != NULL)
    {
        PCB *temp = queue;
        queue = queue->next;
        free(temp);
    }
}

int main()
{
    PCB *readyQueue = NULL;
    loadTasks("task.txt", &readyQueue); // 从文件加载任务

    // 用户选择调度算法
    char algo[10];
    printf("请输入调度算法 (sjf, psjf, pprio, prio): ");
    scanf("%s", algo);

    // 根据用户输入执行相应的调度算法
    if (strcmp(algo, "sjf") == 0)
    {
        SJF(readyQueue);
    }
    else if (strcmp(algo, "psjf") == 0)
    {
        PSJF(readyQueue);
    }
    else if (strcmp(algo, "pprio") == 0)
    {
        PPrio(readyQueue);
    }
    else if (strcmp(algo, "prio") == 0)
    {
        Prio(readyQueue);
    }
    else
    {
        printf("无效的调度算法。\n");
    }

    // 清理内存
    freeMemory(readyQueue);
    freeMemory(waitingQueue);

    return 0;
}

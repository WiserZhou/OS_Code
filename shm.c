#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int segment_id;
    char *shared_memory;
    const int size = 4096;

    // 创建共享内存段
    segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
    // 将共享内存附加到进程的地址空间
    shared_memory = (char *)shmat(segment_id, NULL, 0);

    // 创建子进程
    pid_t pid = fork();

    if (pid == 0)
    { // 子进程
        // 重新附加共享内存
        shared_memory = (char *)shmat(segment_id, NULL, 0);
        printf("Child Write: Hi There!\n");
        sprintf(shared_memory, "Hi There!");
    }
    else
    { // 父进程
        wait(NULL);
        printf("Parent Read: %s\n", shared_memory);
        // 分离共享内存
        shmdt(shared_memory);
        // 删除共享内存段
        shmctl(segment_id, IPC_RMID, NULL);
    }

    // 分离共享内存
    shmdt(shared_memory);
    // 删除共享内存段
    shmctl(segment_id, IPC_RMID, NULL);

    return 0;
}
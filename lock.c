boolean lock = FALSE;
boolean waiting[N] = {FALSE, FALSE, ..., FALSE}; // N个进程

PROCESS i:
while (true) {
    waiting[i] = TRUE; // 表示进程i希望进入临界区
    key = TRUE;        // 设置key为TRUE
    while (waiting[i] && key) {
        key = TestAndSet(&lock); // 尝试获取锁
    }
    waiting[i] = FALSE; // 成功获取锁，清除等待标志

    // 临界区
    critical_section();

    // 释放锁前，确保下一个等待的进程可以进入
    j = (i + 1) % N; // 查找下一个进程
    while ((j != i) && !waiting[j]) {
        j = (j + 1) % N;
    }
    if (j == i) {
        lock = FALSE; // 没有其他进程等待，释放锁
    } else {
        waiting[j] = FALSE; // 允许下一个等待的进程进入
    }

    // 剩余部分
    remainder_section();
}

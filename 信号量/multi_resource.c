#include "semaphore.h"

Semaphore mutex;

void init() {
    mutex.value = 2; // 初始化为2，表示有2个资源
}

void P1() {
    while(1) {
        wait(&mutex);
        // 临界区
        signal(&mutex);
        // 剩余部分
    }
}

void P2() {
    while(1) {
        wait(&mutex);
        // 临界区
        signal(&mutex);
        // 剩余部分
    }
}

void P3() {
    while(1) {
        wait(&mutex);
        // 临界区
        signal(&mutex);
        // 剩余部分
    }
} 
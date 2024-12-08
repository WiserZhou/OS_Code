#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void) {
    void *plib;
    typedef void (*FUN_HELLO)();
    FUN_HELLO funHello = NULL;

    printf("Before loading dynamic library\n");
    printf("Please enter to continue:");
    getchar();

    // 打开动态库
    plib = dlopen("./libtest.so", RTLD_NOW | RTLD_GLOBAL);
    if (plib == NULL) {
        printf("error\n");
        return -1;
    }

    // 获取函数地址
    funHello = dlsym(plib, "printHello");
    
    // 调用动态库中的函数
    funHello();
    
    // 关闭动态库
    dlclose(plib);
    
    printf("After release dynmic library\n");
    printf("Please enter to continue:");
    getchar();

    return 0;
} 
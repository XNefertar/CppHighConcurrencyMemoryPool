#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    // 定义需要分配的内存大小
    size_t size = 100 * 1024; // 100KB，保证小于128KB

    // 使用 brk 来扩展堆
    void* current_brk = sbrk(0); // 获取当前堆的结束位置
    void* new_brk = sbrk(size);  // 扩展堆空间，分配100KB

    if (new_brk == (void*)-1) {
        perror("Failed to increase heap with sbrk");
        return 1;
    }

    // 获取扩展后的堆的结束位置
    printf("Heap extended by %zu bytes\n", size);

    // 现在访问这个新分配的内存区域，它应该会触发缺页中断（如果没有提前初始化）
    char* memory = (char*)current_brk; // 当前 brk 结束位置
    printf("Accessing allocated memory...\n");

    // 第一次访问会导致缺页中断，并映射物理内存
    memory[0] = 'A';  // 这里会触发缺页中断，操作系统分配物理内存

    // 输出修改后的内存值
    printf("Value at memory[0]: %c\n", memory[0]);
    sleep(3);
    return 0;
}

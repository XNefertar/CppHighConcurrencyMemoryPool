#ifndef __ALLOCMEMORYDEMO_HPP__
#define __ALLOCMEMORYDEMO_HPP__

#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// 在堆上按页分配内存
inline static void *SystemAllocMemory(size_t size) {
#ifdef _WIN32
    void *ptr = VirtualAlloc(nullptr, size * (1 << 12), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    void *ptr = mmap(nullptr, size * (1 << 12), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("Failed to allocate memory with mmap");
        return nullptr;
    }
#endif
    if(ptr == nullptr) {
        perror("Failed to allocate memory with VirtualAlloc");
        return nullptr;
    }
    return ptr;
}



template <typename T>
class ObjectPool {
    typedef char* MEMORY_PTR;
    typedef int   SIZE_TYPE;
    typedef void* FREE_LIST;

private:
    MEMORY_PTR _memory    = nullptr;
    FREE_LIST  _free_list = nullptr;
    SIZE_TYPE  _LeftBytes = 0;

public:
    ObjectPool(size_t size) : size_(size) {
        pool_ = (T*)SystemAllocMemory(size_);
    }

    ~ObjectPool() {
        if (pool_ != nullptr) {
        }
    }

    T *New(){
        T *obj = nullptr;
        if (free_list_ != nullptr) {
            obj = (T*)free_list_;
            free_list_ = *(void**)_free_list;
        } 
        else {
            if (current_ < size_) {
                obj = &pool_[current_++];
            }
        }
    }
};



#endif // __ALLOCMEMORYDEMO_HPP__
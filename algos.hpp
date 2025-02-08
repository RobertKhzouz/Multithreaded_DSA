#ifndef ALGOS_H
#define ALGOS_H
#include "structs.h"
#include <atomic>

extern std::atomic<bool> found;
extern std::atomic<int> foundIndex;

template <typename T>
void *linearSearch(void *arguments)
{
    LinearSearchArgs<T> *args = static_cast<LinearSearchArgs<T> *>(arguments);
    for (int i = args->startI; i < args->endI; i++)
    {
        if (found.load())
        {
            return nullptr;
        }
        if (args->arr[i] == args->searchVal)
        {
            foundIndex.store(i, std::memory_order_relaxed);
            found.store(true, std::memory_order_relaxed);
            return nullptr;
        }
    }
    return nullptr;
}

#endif
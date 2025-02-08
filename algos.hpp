#ifndef ALGOS_H
#define ALGOS_H
#include "structs.h"
#include <atomic>
#include <mach/thread_policy.h>
#include <mach/mach.h>

/* MACOS DOES NOT SUPPORT THREAD AFFINITY -- Alternative is to encourage grouping via Affinity Tags */

extern std::atomic<int> foundIndex;

template <typename T>
void *linearSearch(void *arguments)
{
    LinearSearchArgs<T> *args = static_cast<LinearSearchArgs<T> *>(arguments);

    /* Set affinity policy -- cuts down time by over 12 ms...sometimes */
    int affinityTag = args->affinityTag;
    thread_affinity_policy_data_t policy = {affinityTag};
    thread_policy_set(mach_thread_self(), THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);

    for (int i = args->startI; i < args->endI; i++)
    {
        // Another thread has found the value
        if (foundIndex.load(std::memory_order_acquire) != -1)
        {
            return nullptr;
        }
        // This thread has found the value
        if (args->arr[i] == args->searchVal)
        {
            foundIndex.store(i, std::memory_order_relaxed);
            return nullptr;
        }
    }
    // No thread found the value
    return nullptr;
}

#endif
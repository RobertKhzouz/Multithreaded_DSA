#ifndef ALGOS_H
#define ALGOS_H
#include "structs.h"
#include "helpers.h"
#include <unistd.h>
#include <random>
#include <atomic>
#include <mach/thread_policy.h>
#include <mach/mach.h>

/* MACOS DOES NOT SUPPORT MANUAL THREAD AFFINITY -- Alternative is to encourage grouping via Affinity Tags */

std::atomic<int> foundIndex(-1);
std::atomic<pthread_t> foundThread(NULL);

const int DIST_LOWERBOUND = 1;
const int DIST_UPPERBOUND = 100;

/* LINEAR SEARCH ---------------------------------------------------------------------------------------------------------*/
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
            return nullptr;

        // This thread has found the value
        if (args->arr[i] == args->searchVal)
        {
            foundIndex.store(i, std::memory_order_relaxed);
            foundThread.store(pthread_self());
            return nullptr;
        }
    }
    // No thread found the value
    return nullptr;
}

template <typename T>
void linearSearchWrapper(const int threadCount, const int arrSize, const T searchVal)
{
    const int sizePerThread = arrSize / threadCount;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(DIST_LOWERBOUND, DIST_UPPERBOUND);

    // VLA not good, oops, using Vector to prevent Stack Overflow
    std::vector<T> arr(arrSize);

    pthread_t threads[threadCount];
    int affinityTags[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        affinityTags[i] = i;
    }
    std::vector<LinearSearchArgs<T>> argArr(threadCount);

    // Random array
    for (int i = 0; i < arrSize; i++)
    {
        // Genuinely despise this random number generator
        if constexpr (std::is_same<T, std::string>::value)
        {
            arr.at(i) = std::to_string(dist(rng));
        }
        else
        {
            arr.at(i) = static_cast<T>(dist(rng));
        }
    }

    // Initialize arguments -- faster than creation in loop
    for (int i = 0; i < threadCount; i++)
    {
        argArr[i] = LinearSearchArgs<T>{arr.data(), i * sizePerThread, (i + 1) * sizePerThread, searchVal, affinityTags[i]};
    }

    // Thread creation + joining
    for (int i = 0; i < threadCount; i++)
    {
        int errorCode = pthread_create(&threads[i], nullptr, linearSearch<T>, &argArr[i]);
        if (errorCode)
        {
            std::cerr << "Error: Unable to create thread " << i << " (Error Code: " << errorCode << ")\n";
            exit(EXIT_FAILURE);
        }
    }
    for (pthread_t thread : threads)
    {
        pthread_join(thread, nullptr);
    }

    // Atomic read
    int index = foundIndex.load();
    if (index != -1)
    {
        std::cout << "Value found at index: " << index << " in thread " << foundThread.load() << std::endl;
        std::cout << "Indeed, the value at index " << index << " is " << arr[index] << std::endl;
    }
    else
    {
        std::cout << "Value not found!" << std::endl;
    }

    // Reset atomics
    foundIndex.store(-1);
    foundThread.store(NULL);
}

/* BINARY SEARCH ---------------------------------------------------------------------------------------------------------*/
template <typename T>
void *binarySearch(void *arguments)
{
    BinarySearchArgs<T> *args = static_cast<BinarySearchArgs<T> *>(arguments);
    int affinityTag = args->affinityTag;
    thread_affinity_policy_data_t policy = {affinityTag};
    thread_policy_set(mach_thread_self(), THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);

    int start = args->startI;
    int end = args->endI;

    if (args->searchVal < args->arr[start] || args->searchVal > args->arr[end])
        return nullptr;

    while (start <= end)
    {
        if (foundIndex.load(std::memory_order_acquire) != -1)
            return nullptr;

        int mid = ((end + start) / 2);
        if (args->arr[mid] == args->searchVal)
        {
            foundIndex.store(mid, std::memory_order_relaxed);
            foundThread.store(pthread_self());
            return nullptr;
        }

        if (args->arr[mid] < args->searchVal)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }
    return nullptr;
}

template <typename T>
void binarySearchWrapper(const int threadCount, const int arrSize, const T searchVal)
{
    const int sizePerThread = arrSize / threadCount;
    std::vector<T> arr(arrSize);
    pthread_t threads[threadCount];
    int affinityTags[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        affinityTags[i] = i;
    }
    std::vector<BinarySearchArgs<T>> argArr(threadCount);

    for (int i = 0; i < arrSize; i++)
    {
        arr[i] = i;
    }

    for (int i = 0; i < threadCount; i++)
    {
        argArr[i] = BinarySearchArgs<T>{arr.data(), i * sizePerThread, (i + 1) * sizePerThread, searchVal, affinityTags[i]};
    }

    for (int i = 0; i < threadCount; i++)
    {
        int errorCode = pthread_create(&threads[i], nullptr, binarySearch<T>, &argArr[i]);
        if (errorCode)
        {
            std::cerr << "Error: Unable to create thread " << i << " (Error Code: " << errorCode << ")\n";
            exit(EXIT_FAILURE);
        }
    }
    for (pthread_t thread : threads)
    {
        pthread_join(thread, nullptr);
    }

    int index = foundIndex.load();
    if (index != -1)
    {
        std::cout << "Value found at index: " << index << " in thread " << foundThread.load() << std::endl;
        std::cout << "Indeed, the value at index " << index << " is " << arr[index] << std::endl;
    }
    else
    {
        std::cout << "Value not found!" << std::endl;
    }

    foundIndex.store(-1);
    foundThread.store(NULL);

    // printArray(arr.data(), arrSize);
}

#endif
#include <iostream>
#include <pthread.h>
#include <thread>
#include <atomic>
#include <sys/mman.h>
#include "algos.hpp"
#include "helpers.h"
#include "structs.h"

const int ARRSIZE = 1000000;
const int THREAD_COUNT = 4;
const int SIZE_PER_THREAD = ARRSIZE / THREAD_COUNT;
const int SEARCH_VAL = -1;

std::atomic<bool> found(false);
std::atomic<int> foundIndex(-1);

int main()
{
    /*
    If for any reason the program is suffering due to cache eviction, enable this line
    std::this_thread::sleep_for(std::chrono::seconds(10));
    */

    auto start = std::chrono::high_resolution_clock::now();
    int arr[ARRSIZE];
    pthread_t threads[THREAD_COUNT];
    int affinityTags[THREAD_COUNT] = {0, 1, 2, 3};
    LinearSearchArgs<int> argArr[THREAD_COUNT];
    srand(time(NULL));

    // Random array
    for (int i = 0; i < ARRSIZE; i++)
    {
        arr[i] = rand() % 100000;
    }

    // Initialize arguments -- faster than creation in loop
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        argArr[i] = LinearSearchArgs<int>{arr, i * SIZE_PER_THREAD, (i + 1) * SIZE_PER_THREAD, SEARCH_VAL, affinityTags[i]};
    }

    // Thread creation + joining
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], nullptr, linearSearch<int>, &argArr[i]);
    }
    for (pthread_t thread : threads)
    {
        pthread_join(thread, nullptr);
    }

    // Atomic read
    if (found.load())
    {
        std::cout << "Value found at index: " << foundIndex.load() << std::endl;
    }
    else
    {
        std::cout << "Value not found!" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}
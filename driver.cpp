#include <iostream>
#include <math.h>
#include <pthread.h>
#include <atomic>
#include "algos.hpp"
#include "helpers.h"
#include "structs.h"

const int ARRSIZE = 1000000;
const int THREAD_COUNT = 4;
const int SEARCH_VAL = -1;

std::atomic<bool> found(false);
std::atomic<int> foundIndex(-1);

int main()
{
    int arr[ARRSIZE];
    srand(time(NULL));
    for (int i = 0; i < ARRSIZE; i++)
    {
        arr[i] = rand() % 100000;
    }
    pthread_t threads[THREAD_COUNT];

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        LinearSearchArgs<int> args = {arr, i * (ARRSIZE / THREAD_COUNT), (i + 1) * (ARRSIZE / THREAD_COUNT), SEARCH_VAL};
        pthread_create(&threads[i], nullptr, linearSearch<int>, &args);
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], nullptr);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    if (found.load())
    {
        std::cout << "Value found at index: " << foundIndex.load() << std::endl;
    }
    else
    {
        std::cout << "Value not found!" << std::endl;
    }

    return 0;
}
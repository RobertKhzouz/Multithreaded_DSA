#include <iostream>
#include <math.h>
#include <pthread.h>
#include <atomic>
#include "algos.hpp"
#include "helpers.h"
#include "structs.h"

const int ARRSIZE = 21;
const int HALFARR_CEILING = ceil(double(ARRSIZE) / 2);
const int HALFARR_FLOOR = floor(double(ARRSIZE) / 2);
const int THREAD_COUNT = 2;
const int SEARCH_VAL = 0;

std::atomic<bool> found(false);
std::atomic<int> foundIndex(-1);

int main()
{
    pthread_t t1;
    pthread_t t2;
    int arr[ARRSIZE] = {4, 11, 9, 30, 31, 2, 7, 9, 15, 14, 10, 65, 32, 87, 39, 54, 1, 90, 43, 21, 55};
    LinearSearchArgs<int> args1 = {arr, 0, HALFARR_FLOOR, SEARCH_VAL};
    LinearSearchArgs<int> args2 = {arr, HALFARR_FLOOR, ARRSIZE, SEARCH_VAL};

    auto start = std::chrono::high_resolution_clock::now();
    pthread_create(&t1, nullptr, linearSearch<int>, &args1);
    pthread_create(&t2, nullptr, linearSearch<int>, &args2);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
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
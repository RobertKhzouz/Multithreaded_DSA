#include <iostream>
#include <pthread.h>
#include <thread>
#include <atomic>
#include <sys/mman.h>
#include "algos.hpp"
#include "helpers.h"
#include "structs.h"

int main()
{
    /*
    If for any reason the program is suffering due to cache eviction, enable this line
    std::this_thread::sleep_for(std::chrono::seconds(10));
    */

    auto start = std::chrono::high_resolution_clock::now();

    linearSearchWrapper(4, 100, 10);
    linearSearchWrapper(4, 100, 10);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}
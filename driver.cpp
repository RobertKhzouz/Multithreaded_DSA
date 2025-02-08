#include <iostream>
#include "algos.h"
#include "helpers.h"
#include <math.h>

const int ARRSIZE = 10;
const int HALFARR_CEILING = ceil(double(ARRSIZE) / 2);
const int HALFARR_FLOOR = floor(ARRSIZE / 2);

int main()
{

    int arr[ARRSIZE] = {4, 11, 9, 30, 31, 2, 7, 9, 15, 14};

    int arr1[HALFARR_FLOOR];
    int arr2[HALFARR_CEILING];

    splitArray<int>(arr, ARRSIZE, arr1, arr2);
    printArray<int>(arr1, HALFARR_FLOOR);
    printArray<int>(arr2, HALFARR_CEILING);

    std::cout << linearSearch(arr, ARRSIZE, 0) << std::endl;
    return 0;
}
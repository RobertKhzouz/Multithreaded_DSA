#ifndef HELPERS_H
#define HELPERS_H

// Takes an array and splits its contents in half into array first and array second
// Second array will always take dangling odd element
// Ex. if size is 11, first will have 5, second will have 6
template <typename T>
void splitArray(const T *original, const int arrSize, T *first, T *second)
{
    for (int i = 0; i < arrSize / 2; i++)
    {
        first[i] = original[i];
    }
    for (int i = 0, j = arrSize / 2; j < arrSize; i++, j++)
    {
        second[i] = original[j];
    }
}

// Prints array...kinda self explanatory
template <typename T>
void printArray(T *arr, int size, std::string name = "ARR")
{
    std::cout << name << ": ";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl;
}

#endif
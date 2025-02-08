#ifndef ALGOS_H
#define ALGOS_H

template <typename T>
int linearSearch(T *arr, int size, T searchVal)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == searchVal)
        {
            return i;
        }
    }
    return -1;
}

#endif
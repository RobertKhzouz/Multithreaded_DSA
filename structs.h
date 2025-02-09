#ifndef STRUCTS_H
#define STRUCTS_H

// LINEAR SEARCH
template <typename T>
struct LinearSearchArgs
{
    T *arr;
    int startI;
    int endI;
    T searchVal;
    int affinityTag;
};

// BINARY SEARCH
template <typename T>
struct BinarySearchArgs
{
    T *arr;
    int startI;
    int endI;
    T searchVal;
    int affinityTag;
};

#endif
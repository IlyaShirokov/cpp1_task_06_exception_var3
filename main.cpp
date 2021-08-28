#include <iostream>

#include "ordered_vector.h"

int main ()
{
    OrderedVectorImpl<int> v1;
    OrderedVectorImpl<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    OrderedVectorImpl<int> v3 = v2;
    OrderedVectorImpl<int> v4;
    v4 = v3;
    OrderedVectorImpl<int> v5 (std::move(v4));
    std::cout << "or" << std::endl;
    int *a = new int[10000];
    for (int i = 0; i < 10000; ++i)
    {
        a[i] = i*i;
    }
    delete[] a;
    std::cout << "or" << std::endl;
}

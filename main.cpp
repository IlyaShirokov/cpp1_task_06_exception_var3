#include <iostream>

#include "ordered_vector.h"

int main ()
{
//    std::cout << "--------------Defaul constructor--------------" << std::endl;
//    OrderedVector<int> v1;
//    std::cout << v1 << std::endl;
//    OrderedVector<int> v2(10);
//    std::cout << v2 << std::endl;

//    std::cout << "--------------Constructor std::initializer_list--------------" << std::endl;
//    OrderedVector<int> v1 = {12, 45, 123, 887, 1111, 34};
//    std::cout << v1 << std::endl;

//    std::cout << "--------------Constructor r-value--------------" << std::endl;
//    OrderedVector<int> v1 = {12, 45, 123, 887, 1111, 34};
//    std::cout << v1 << std::endl;
//    OrderedVector<int> v2(std::move(v1));
//    std::cout << v1 << std::endl;
//    std::cout << v2 << std::endl;

//    std::cout << "--------------Copy constructor--------------" << std::endl;
//    OrderedVector<int> v1 = {12, 45, 123, 887, 1111, 34};
//    std::cout << v1 << std::endl;
//    OrderedVector<int> v2(v1);
//    std::cout << v1 << std::endl;
//    std::cout << v2 << std::endl;

    std::cout << "--------------iterator--------------" << std::endl;
    OrderedVector<int> v1 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<const int>::base_iterator it = v1.cbegin();
    std::cout << *(it+1) << " ";
    std::cout << *(it+2) << " ";
    std::cout << *(it+3) << " ";

//    std::cout << "--------------const_iterator--------------" << std::endl;
//    std::cout << "--------------insert(T&& element)--------------" << std::endl;
//    std::cout << "--------------insert(const T& element)--------------" << std::endl;
//    std::cout << "--------------erase(iterator& iter)--------------" << std::endl;
//    std::cout << "--------------clear()--------------" << std::endl;

//    std::cout << "--------------operator=(OrderedVector<T>&&--------------" << std::endl;
//    std::cout << "--------------operator=(const OrderedVector<T>&--------------" << std::endl;

//    std::cout << "--------------operator[]--------------" << std::endl;

//    std::cout << "--------------iterator begin()--------------" << std::endl;
//    std::cout << "--------------const_iterator cbegin()--------------" << std::endl;
//    std::cout << "--------------iterator end()--------------" << std::endl;
//    std::cout << "--------------const_iterator cend()--------------" << std::endl;

//    std::cout << "--------------explicit base_iterator(T* pT)--------------" << std::endl;
//    std::cout << "--------------base_iterator(const base_iterator& rIt)--------------" << std::endl;

//    std::cout << "--------------operator==(const base_iterator& other)--------------" << std::endl;
//    std::cout << "--------------bool operator!=(const base_iterator& other)--------------" << std::endl;

//    std::cout << "--------------T &operator*();--------------" << std::endl;
//    std::cout << "--------------const T &operator*() const;--------------" << std::endl;
//    std::cout << "--------------T* operator->();--------------" << std::endl;

//    std::cout << "--------------base_iterator operator+(int n) const;--------------" << std::endl;
//    std::cout << "--------------base_iterator operator-(int n) const;--------------" << std::endl;
//    std::cout << "--------------base_iterator& operator++();--------------" << std::endl;
//    std::cout << "--------------base_iterator operator++(int);--------------" << std::endl;
//    std::cout << "--------------base_iterator& operator--();--------------" << std::endl;
//    std::cout << "--------------base_iterator operator--(int);--------------" << std::endl;
}

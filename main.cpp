#include <iostream>

#include "ordered_vector.h"

void baseType_test();
void customType_test();

struct Test
{
    int m_n;
    int m_nCheck;
    bool m_bThrowInMove, m_bThrowLater;
    static bool ms_bThrowDeferred;

    Test(int n, bool bThrowInMove = false, bool bThrowLater = false)
        : m_n(n),
        m_nCheck(1),
        m_bThrowInMove(bThrowInMove),
        m_bThrowLater(bThrowLater)
    {
        std::cout << "Test(" << n << ")" << std::endl;
    }

//    Test(Test&& rrTest)
//        : m_n(std::move(rrTest.m_n)),
//        m_nCheck(std::move(rrTest.m_nCheck)),
//        m_bThrowInMove(std::move(rrTest.m_bThrowInMove)),
//        m_bThrowLater(std::move(rrTest.m_bThrowLater))
//    {
//        std::cout << "Test(Test&& rrTest) << std::endl;
//    }

    Test(const Test &rcTest)
        : m_n(rcTest.m_n),
        m_nCheck(rcTest.m_nCheck),
        m_bThrowInMove(rcTest.m_bThrowInMove),
        m_bThrowLater(rcTest.m_bThrowLater)
    {
        std::cout << "Test(const Test &rcTest), rcTest == " << rcTest << std::endl;
        if (m_bThrowLater && ms_bThrowDeferred)
        {
            std::cout << "Throwing..." << std::endl;
            throw std::logic_error(std::string("Test(Test &&)"));
        }
        if (m_bThrowInMove)
        {
            std::cout << "Throwing..." << std::endl;
            throw std::logic_error(std::string("Test(") + std::to_string(m_n) + ")");
        }
    }

    friend std::ostream &operator << (std::ostream &rStream, const Test &rcTest)
    {
        rStream << rcTest.m_n;
        return rStream;
    }

    bool operator<(const Test& rTest) const
    {
        return m_n < rTest.m_n;
    }

    Test &operator = (const Test &) = delete;
    Test &operator = (Test &&rTest)
    {
        if (this != &rTest)
        {
            m_n = std::forward<int>(rTest.m_n);
            m_nCheck = std::forward<int>(rTest.m_nCheck);
            m_bThrowInMove = std::forward<bool>(rTest.m_bThrowInMove);
            m_bThrowLater = std::forward<bool>(rTest.m_bThrowLater);
        }
        return *this;
    }
};
bool Test::ms_bThrowDeferred = false;

int main ()
{
    baseType_test();
    customType_test();
}

void customType_test()
{
    std::cout << "Test 1" << std::endl;
    std::cout << "1. insert()" << std::endl;
    OrderedVector<Test> container;
    int i;
    for (i = 0; i < 3; ++i)
        container.insert(Test(i));
    std::cout << container << std::endl;

    try
    {
        container.insert(Test(999, true));
    }
    catch (const std::exception &rcException)
    {
        std::cout << "Exception: " << rcException.what();
    }

    std::cout << container << std::endl;

    container.insert(Test(i++));

    std::cout << container << std::endl;

    try
    {
        container.insert(Test(9999, true));
    }
    catch (const std::exception &rcException)
    {
        std::cout << "Exception: " << rcException.what() << std::endl;
    }

    std::cout << container << std::endl;

    container.insert(Test(i++, false, true));
    container.insert(Test(i++));

    std::cout << container << std::endl;

    std::cout << "2. Copy construction" << std::endl;
    Test::ms_bThrowDeferred = true;
    try
    {
        OrderedVector<Test> s2 = container;
        s2.insert(Test(i++, false, true));
    }
    catch (const std::exception &rcException)
    {
        std::cout << "Exception: " << rcException.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "catch (...)" << std::endl;
    }

    std::cout << "3. Copy assignment" << std::endl;
    OrderedVector<Test> s3;
    s3.insert(Test(111));
    try
    {
        s3 = container;
    }
    catch (const std::exception &rcException)
    {
        std::cout << "Exception: " << rcException.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "catch (...)" << std::endl;
    }
    std::cout << s3 << std::endl;
}

void baseType_test()
{
    std::cout << "--------------Defaul constructor--------------" << std::endl;
    OrderedVector<int> v1;
    std::cout << v1 << std::endl;
    OrderedVector<int> v2(10);
    std::cout << v2 << std::endl;

    std::cout << "--------------Constructor std::initializer_list--------------" << std::endl;
    OrderedVector<int> v3 = {12, 45, 123, 887, 1111, 34};
    std::cout << v3 << std::endl;

    std::cout << "--------------Constructor r-value--------------" << std::endl;
    OrderedVector<int> v4 = {12, 45, 123, 887, 1111, 34};
    std::cout << v4 << std::endl;
    OrderedVector<int> v5(std::move(v4));
    std::cout << v4 << std::endl;
    std::cout << v5 << std::endl;

    std::cout << "--------------Copy constructor--------------" << std::endl;
    OrderedVector<int> v6 = {12, 45, 123, 887, 1111, 34};
    std::cout << v6 << std::endl;
    OrderedVector<int> v7(v6);
    std::cout << v6 << std::endl;
    std::cout << v7 << std::endl;

    std::cout << "--------------iterator--------------" << std::endl;
    OrderedVector<int> v8 = {12, 45, 123, 887, 1111, 34};
    std::cout << v8 << std::endl;
    OrderedVector<int>::iterator it = v8.begin();
    std::cout << *(it + 1) << " ";
    std::cout << *(it + 2) << " ";
    std::cout << *(it + 3) << " ";

    std::cout << "--------------const_iterator--------------" << std::endl;
    OrderedVector<int> v9 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<const int>::const_iterator it1 = v9.cbegin();
    std::cout << *(it1 + 1) << " ";
    std::cout << *(it1 + 2) << " ";
    std::cout << *(it1 + 3) << " ";

    std::cout << "--------------base_iterator find(const T& other)--------------" << std::endl;
    OrderedVector<int> v10 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<int> v11;
    v11 = v10;
    std::cout << v10 << std::endl;
    std::cout << v11 << std::endl;
    OrderedVector<int>::iterator it2 = v10.begin();
    it2 = v10.find(999);
    std::cout << *it2 << std::endl;
    std::cout << v10 << std::endl;
    std::cout << v11 << std::endl;

    std::cout << "--------------insert(T&& element)--------------" << std::endl;
    OrderedVector<int> v12 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<int> v13;
    v13 = v12;
    std::cout << v12 << std::endl;
    std::cout << v13 << std::endl;
    v12.insert(567);
    std::cout << v12 << std::endl;
    std::cout << v13 << std::endl;

    std::cout << "--------------erase(iterator& iter)--------------" << std::endl;
    OrderedVector<int> v14 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<int> v15;
    v15 = v14;
    std::cout << v14 << std::endl;
    std::cout << v15 << std::endl;
    OrderedVector<int>::iterator it3 = v14.find(887);
    std::cout << *it3 << std::endl;
    v14.erase(it3);
    std::cout << *it3 << std::endl;
    std::cout << v14 << std::endl;
    std::cout << v15 << std::endl;

    std::cout << "--------------clear()--------------" << std::endl;
    OrderedVector<int> v16 = {12, 45, 123, 887, 1111, 34};
    OrderedVector<int> v17;
    v17 = v16;
    std::cout << v16 << std::endl;
    std::cout << v17 << std::endl;
    v16.clear();
    std::cout << v16 << std::endl;
    std::cout << v17 << std::endl;

    std::cout << "--------------operator[]--------------" << std::endl;
    OrderedVector<int> v18 = {12, 45, 123, 887, 1111, 34};
    std::cout << v18[0] << std::endl;
    std::cout << v18[2] << std::endl;
    std::cout << v18[8] << std::endl;
}

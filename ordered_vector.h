#ifndef ORDERED_VECTOR_H
#define ORDERED_VECTOR_H

#include <iostream>
#include <algorithm>
#include <memory>

template <class T1, class T2>
void construct(T1 *pT1, const T2 &&rrT2)
{
    new (pT1) T1(std::move(rrT2));
}

template <class T> void destroy(T *pT)
{
    pT->~T();
}

template <class FwdIter>
void destroy(FwdIter first, FwdIter last)
{
    while (first != last)
    {
        destroy(&*first);
        ++ first;
    }
}

template <class T>
class OrderedVectorImpl
{
public:
    OrderedVectorImpl(size_t capacity = 0);
    OrderedVectorImpl(std::initializer_list<T>& lst);
    OrderedVectorImpl(OrderedVectorImpl<T>&& rrOtherVecImpl);
    OrderedVectorImpl(OrderedVectorImpl<T>& rOtherVecImpl);
    ~OrderedVectorImpl();

    OrderedVectorImpl<T>& operator= (const OrderedVectorImpl<T>& rOtherVecImpl);

    void swap(OrderedVectorImpl<T>& rOtherVecImpl) noexcept;
    OrderedVectorImpl<T>* clone();

    size_t size() const;
    size_t capacity() const;
    bool is_DataChanged() const;
    void sort();

private:
    T* m_arr;
    size_t m_capacity;
    size_t m_used;
    bool m_dataChanged;
};

template <class T>
OrderedVectorImpl<T>::OrderedVectorImpl(size_t capacity) :
    m_arr (static_cast<T*>(capacity == 0 ? nullptr : operator new (sizeof (T) * capacity))),
    m_capacity (capacity),
    m_used (0),
    m_dataChanged (false)
{
    std::cout << "Default constructor of OrderedVectorImpl" << std::endl;
}

template <class T>
OrderedVectorImpl<T>::OrderedVectorImpl(std::initializer_list<T>& lst) :
    m_arr (static_cast<T*>(lst.size() == 0 ? nullptr : operator new (sizeof (T) * lst.size()))),
    m_capacity (lst.size()),
    m_used (0),
    m_dataChanged (false)
{
    std::cout << "Constructor std::initializer_list of OrderedVectorImpl" << std::endl;

    for (auto& item : lst)
    {
        construct (m_arr + m_used, std::move(item));
        m_used++;
    }
}

template <class T>
OrderedVectorImpl<T>::OrderedVectorImpl(OrderedVectorImpl<T>&& rrOtherVecImpl) :
    m_arr(std::move(rrOtherVecImpl.m_arr)),
    m_capacity(std::move(rrOtherVecImpl.m_capacity)),
    m_used(std::move(rrOtherVecImpl.m_used)),
    m_dataChanged(std::move(rrOtherVecImpl.m_dataChanged))
{
    std::cout << "Move constructor of OrderedVectorImpl" << std::endl;
}

template <class T>
OrderedVectorImpl<T>::OrderedVectorImpl(OrderedVectorImpl<T>& rOtherVecImpl) :
    m_arr (static_cast<T*>(rOtherVecImpl.m_capacity == 0 ? nullptr : operator new (sizeof (T) * rOtherVecImpl.m_capacity))),
    m_capacity (rOtherVecImpl.m_capacity),
    m_used (0),
    m_dataChanged (false)
{
    std::cout << "Copy constructor of OrderedVectorImpl" << std::endl;

    while (m_used < rOtherVecImpl.m_used)
    {
        construct (m_arr + m_used, *(rOtherVecImpl.m_arr + m_used));
        m_used++;
    }
}

template <class T>
OrderedVectorImpl<T>::~OrderedVectorImpl()
{
    std::cout << "Destructor of OrderedVectorImpl" << std::endl;

    destroy(m_arr, m_arr + m_used);
    operator delete (m_arr);
}

template <class T>
OrderedVectorImpl<T>& OrderedVectorImpl<T>::operator= (const OrderedVectorImpl<T>& rOtherVecImpl)
{
    std::cout << "operator= of OrderedVectorImpl" << std::endl;
    if (this != &rOtherVecImpl)
    {
        m_arr = rOtherVecImpl.m_arr;
        m_capacity = rOtherVecImpl.m_capacity;
        m_used = rOtherVecImpl.m_used;
        m_dataChanged = rOtherVecImpl.m_dataChanged;
    }
    return *this;
}

template <class T>
void OrderedVectorImpl<T>::swap(OrderedVectorImpl<T>& rOtherVecImpl) noexcept
{
    if (this != &rOtherVecImpl)
    {
        std::swap(m_arr, rOtherVecImpl.m_arr);
        std::swap(m_capacity, rOtherVecImpl.m_capacity);
        std::swap(m_used, rOtherVecImpl.m_used);
        std::swap(m_dataChanged, rOtherVecImpl.m_dataChanged);
    }
}

template <class T>
OrderedVectorImpl<T>* OrderedVectorImpl<T>::clone()
{
    std::cout << "Clone of OrderedVectorImpl" << std::endl;
    return new OrderedVectorImpl<T>(*this);
}

template <class T>
size_t OrderedVectorImpl<T>::size() const
{
    return m_used;
}

template <class T>
size_t OrderedVectorImpl<T>::capacity() const
{
    return m_capacity;
}

template <class T>
bool OrderedVectorImpl<T>::is_DataChanged() const
{
    return m_dataChanged;
}

template <class T>
void OrderedVectorImpl<T>::sort()
{
    std::sort(m_arr, m_arr + m_used);
}

#endif // ORDERED_VECTOR_H

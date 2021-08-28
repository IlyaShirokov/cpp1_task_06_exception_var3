#ifndef OrderedVector_H
#define OrderedVector_H

#include <iostream>
#include <algorithm>
#include <memory>

template <class T1, class T2>
void construct(T1 *pT1, T2 &&rrT2)
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

template < class T >
class OrderedVector;

template <class T>
class OrderedVectorImpl
{
public:
    OrderedVectorImpl(size_t capacity = 0);
    OrderedVectorImpl(std::initializer_list<T>&& lst);
    OrderedVectorImpl(OrderedVectorImpl<T>&& rrOtherVecImpl);
    OrderedVectorImpl(OrderedVectorImpl<T>& rOtherVecImpl, size_t capacity = 0);
    ~OrderedVectorImpl();

    OrderedVectorImpl<T>& operator= (const OrderedVectorImpl<T>& rOtherVecImpl);

    void swap(OrderedVectorImpl<T>& rOtherVecImpl) noexcept;
    OrderedVectorImpl<T>* clone();

    size_t size() const;
    size_t capacity() const;
    bool is_dataChanged() const;
    void sort();

private:
    friend class OrderedVector<T>;

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
OrderedVectorImpl<T>::OrderedVectorImpl(std::initializer_list<T>&& lst) :
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
OrderedVectorImpl<T>::OrderedVectorImpl(OrderedVectorImpl<T>& rOtherVecImpl, size_t capacity) :
    m_arr(static_cast<T*>(std::max(capacity, rOtherVecImpl.capacity()) ? operator new(sizeof(T) * std::max(capacity, rOtherVecImpl.m_capacity)) : nullptr)),
    m_capacity(std::max(capacity, rOtherVecImpl.m_capacity)),
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
        destroy(m_arr, m_arr + m_used);
        operator delete (m_arr);

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
bool OrderedVectorImpl<T>::is_dataChanged() const
{
    return m_dataChanged;
}

template <class T>
void OrderedVectorImpl<T>::sort()
{
    std::sort(m_arr, m_arr + m_used);
}

template <class T>
class OrderedVector
{
public:
    class base_iterator;
    using iterator = typename OrderedVector<T>::base_iterator;
    using const_iterator = typename OrderedVector<const T>::base_iterator;

    OrderedVector(size_t capacity = 0);
    OrderedVector(std::initializer_list<T>&& lst);
    OrderedVector(OrderedVector<T>&& rrOtherVec);
    OrderedVector(const OrderedVector<T>& rOtherVec);
    ~OrderedVector();

    iterator find(const T& other);
    const_iterator find(const T& other) const;
    void insert(T&& element);
    void insert(const T& element);
    void erase(iterator& iter);
    void clear() noexcept;

    template <class U>
    friend std::ostream& operator<< (std::ostream &out, const OrderedVector<U> &vec);

    bool is_dataChanged() const;
    size_t size() const;
    size_t size_refs() const;
    size_t capacity() const;

    OrderedVector<T>& operator=(OrderedVector<T>&& rrOtherVec) noexcept;
    OrderedVector<T>& operator=(const OrderedVector<T>& rcOtherVec) noexcept;
    T& operator[](size_t i);

    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;

    class base_iterator
    {
    public:
        explicit base_iterator(T* pT);
        base_iterator(const base_iterator& rIt);

        bool operator==(const base_iterator& other) const;
        bool operator!=(const base_iterator& other) const;

        T &operator*();
        const T &operator*() const;
        T* operator->();

        base_iterator operator+(int n) const;
        base_iterator operator-(int n) const;
        base_iterator& operator++();
        base_iterator operator++(int);
        base_iterator& operator--();
        base_iterator operator--(int);

    private:
        T* m_Iter;

    };

private:
    std::shared_ptr<OrderedVectorImpl<T>> m_shrPrtImpl;
};

template <class T>
OrderedVector<T>::OrderedVector(size_t capacity) :
    m_shrPrtImpl(std::make_shared<OrderedVectorImpl<T>>(capacity))
{
    std::cout << "Defaul constructor of OrderedVector" << std::endl;
}

template <class T>
OrderedVector<T>::OrderedVector(std::initializer_list<T>&& lst) :
    m_shrPrtImpl(std::make_shared<OrderedVectorImpl<T>>(std::move(lst)))
{
    std::cout << "Constructor std::initializer_list of OrderedVector" << std::endl;
    m_shrPrtImpl->sort();
}

template <class T>
OrderedVector<T>::OrderedVector(OrderedVector<T>&& rrOtherVec)
{
    std::cout << "Constructor r-value of OrderedVector" << std::endl;
    std::swap(m_shrPrtImpl, rrOtherVec.m_shrPrtImpl);
}

template <class T>
OrderedVector<T>::OrderedVector(const OrderedVector<T>& rOtherVec)
{
    std::cout << "Copy constructor of OrderedVector" << std::endl;
    if (rOtherVec.is_dataChanged())
        m_shrPrtImpl = std::make_shared<OrderedVectorImpl<T>>(*rOtherVec.m_shrPrtImpl);
    else
        m_shrPrtImpl = rOtherVec.m_shrPrtImpl;
    std::cout << "Refs on m_shrPrtImpl: " << m_shrPrtImpl.use_count() << std::endl;
}

template <class T>
OrderedVector<T>::~OrderedVector()
{
    std::cout << "Destructor of OrderedVector" << std::endl;
}

template <class T>
typename OrderedVector<T>::base_iterator OrderedVector<T>::find(const T& other)
{
    for (auto it = begin(); it != end(); ++it)
    {
        if (*it == other)
        {
            if (m_shrPrtImpl.use_count() > 1)
            {
                std::ptrdiff_t diff = m_shrPrtImpl->m_arr - &*it;
                m_shrPrtImpl.reset(new OrderedVectorImpl<T>(*m_shrPrtImpl));
//                std::ptrdiff_t diff = &*it - m_shrPrtImpl->m_pArr;
//                OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl);
//                m_shrPrtImpl.reset(temp);
//                it = typename OrderedVector<T>::iterator(&m_shrPrtImpl->m_arr[diff]);
            }
            m_shrPrtImpl->m_dataChanged = true;
            return it;
        }
    }
    return end();
}

template <class T>
typename OrderedVector<T>::const_iterator OrderedVector<T>::find(const T& other) const
{
    for (auto it = cbegin(); it != cend(); ++it)
        if (*it == other)
            return it;
    return cend();
}

template <class T>
void OrderedVector<T>::insert(T&& element)
{
    std::cout << "insert T&&" << std::endl;
    if (m_shrPrtImpl->capacity() < m_shrPrtImpl->m_used + 1 || m_shrPrtImpl.use_count() > 1)
    {
        size_t capacity_new = m_shrPrtImpl->size() + 1 > m_shrPrtImpl->capacity() ? m_shrPrtImpl->size() + 5 : m_shrPrtImpl->capacity();
        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl, capacity_new);
        construct(temp->m_arr + temp->size(), std::forward<T>(element));
        temp->m_used ++;
        m_shrPrtImpl.reset(temp);
    }
    else
    {
        construct(m_shrPrtImpl->m_arr + m_shrPrtImpl->size(), std::forward<T>(element));
        m_shrPrtImpl->m_used ++;
    }
    m_shrPrtImpl->sort();
}

template <class T>
void OrderedVector<T>::insert(const T& element)
{
    std::cout << "insert const T&" << std::endl;
    if (m_shrPrtImpl->capacity() < m_shrPrtImpl->m_Used + 1 || m_shrPrtImpl.use_count() > 1)
    {
        size_t capacity_new = m_shrPrtImpl->size() + 1 > m_shrPrtImpl->capacity() ? m_shrPrtImpl->size() + 5 : m_shrPrtImpl->capacity();
        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl, capacity_new);
        construct(temp->m_pArr + temp->size(), std::forward<T>(element));
        temp->m_Used ++;
        m_shrPrtImpl.reset(temp);
    }
    else
    {
        construct(m_shrPrtImpl->m_pArr + m_shrPrtImpl->size(), element);
        m_shrPrtImpl->m_Used ++;
    }
    m_shrPrtImpl->sort();
}

template <class T>
void OrderedVector<T>::erase(iterator& iter)
{
    std::ptrdiff_t shift = &*iter - m_shrPrtImpl->m_arr;
    if (m_shrPrtImpl.use_count() > 1)
    {
//        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl);
//        m_shrPrtImpl.reset(temp);
        m_shrPrtImpl.reset(new OrderedVectorImpl<T>(*m_shrPrtImpl));
    }

    std::swap(m_shrPrtImpl->m_arr[shift], m_shrPrtImpl->m_arr[m_shrPrtImpl->m_used - 1]);
    m_shrPrtImpl->m_Used --;
    m_shrPrtImpl->sort();
}

template <class T>
void OrderedVector<T>::clear() noexcept
{
    m_shrPrtImpl.reset();
}

template <class T>
bool OrderedVector<T>::is_dataChanged() const
{
    return m_shrPrtImpl->is_dataChanged();
}

template <class T>
size_t OrderedVector<T>::size() const
{
    return m_shrPrtImpl->m_used;
}

template <class T>
size_t OrderedVector<T>::size_refs() const
{
    return m_shrPrtImpl.use_count();
}

template <class T>
size_t OrderedVector<T>::capacity() const
{
    return m_shrPrtImpl->capacity();
}

template <class T>
OrderedVector<T>& OrderedVector<T>::operator=(OrderedVector<T>&& rrOtherVec) noexcept
{
    std::cout << "OrderedVector: operator= (move)" << std::endl;
    if (this != &rrOtherVec)
    {
        if (m_shrPrtImpl.use_count() > 1)
        {
//            OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>();
//            m_shrPrtImpl.reset(temp);
            m_shrPrtImpl.reset(new OrderedVectorImpl<T>());
        }
        m_shrPrtImpl->swap(*rrOtherVec.m_shrPrtImpl);
    }
    return *this;
}

template <class T>
OrderedVector<T>& OrderedVector<T>::operator=(const OrderedVector<T>& rcOtherVec) noexcept
{
    std::cout << "OrderedVector: operator =" << std::endl;
    if (this != &rcOtherVec)
    {
        if (rcOtherVec.m_shrPrtImpl->is_dataChanged())
            m_shrPrtImpl = std::make_shared<OrderedVectorImpl<T>>(*rcOtherVec.m_shrPrtImpl);
        else
            m_shrPrtImpl = rcOtherVec.m_shrPrtImpl;
    }
    return *this;
}

template <class T>
T& OrderedVector<T>::operator[](size_t i)
{
    if (m_shrPrtImpl.use_count() > 1)
    {
//        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl);
//        m_shrPrtImpl.reset(temp);
        m_shrPrtImpl.reset(new OrderedVectorImpl<T>(*m_shrPrtImpl));
    }
    m_shrPrtImpl->m_dataChanged = true;
    return m_shrPrtImpl->m_arr[i];
}

template <class T>
typename OrderedVector<T>::iterator OrderedVector<T>::begin()
{
    if (m_shrPrtImpl.use_count() > 1)
    {
//        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl);
//        m_shrPrtImpl.reset(temp);
        m_shrPrtImpl.reset(new OrderedVectorImpl<T>(*m_shrPrtImpl));
    }
    m_shrPrtImpl->m_dataChanged = true;
    return OrderedVector<T>::iterator(&m_shrPrtImpl->m_arr[0]);
}

template <class T>
typename OrderedVector<T>::const_iterator OrderedVector<T>::cbegin() const
{
    return OrderedVector<T>::const_iterator(&m_shrPrtImpl->m_arr[0]);
}

template <class T>
typename OrderedVector<T>::iterator OrderedVector<T>::end()
{
    if (m_shrPrtImpl.use_count() > 1)
    {
//        OrderedVectorImpl<T>* temp = new OrderedVectorImpl<T>(*m_shrPrtImpl);
//        m_shrPrtImpl.reset(temp);
        m_shrPrtImpl.reset(new OrderedVectorImpl<T>(*m_shrPrtImpl));
    }
    m_shrPrtImpl->m_dataChanged = true;
    return OrderedVector<T>::iterator(&m_shrPrtImpl->m_arr[0] + m_shrPrtImpl->m_used);
}

template <class T>
typename OrderedVector<T>::const_iterator OrderedVector<T>::cend() const
{
    return OrderedVector<T>::const_iterator(&m_shrPrtImpl->m_arr[0] + m_shrPrtImpl->m_used);
}

template <class T>
OrderedVector<T>::base_iterator::base_iterator(T* pT) :
    m_Iter(pT)
{}
template <class T>
OrderedVector<T>::base_iterator::base_iterator(const base_iterator& rIt) :
    m_Iter(rIt.m_Iter)
{}

template <class T>
bool OrderedVector<T>::iterator::operator==(const base_iterator& other) const
{
    return m_Iter == other.m_Iter;
}

template <class T>
bool OrderedVector<T>::iterator::operator!=(const base_iterator& other) const
{
    return m_Iter != other.m_Iter;
}

template <class T>
T& OrderedVector<T>::iterator::operator*()
{
    return *m_Iter;
}

template <class T>
const T& OrderedVector<T>::iterator::operator*() const
{
    return *m_Iter;
}

template <class T>
T* OrderedVector<T>::iterator::operator->()
{
    return m_Iter;
}

template <class T>
typename OrderedVector<T>::base_iterator OrderedVector<T>::iterator::operator+(int n) const
{
    return base_iterator(m_Iter + n);
}

template <class T>
typename OrderedVector<T>::base_iterator OrderedVector<T>::iterator::operator-(int n) const
{
    return base_iterator(m_Iter - n);
}

template <class T>
typename OrderedVector<T>::base_iterator& OrderedVector<T>::iterator::operator++()
{
    this->m_Iter ++;
    return *this;
}

template <class T>
typename OrderedVector<T>::base_iterator OrderedVector<T>::iterator::operator++(int)
{
    base_iterator tmp(m_Iter ++);
    return tmp;
}

template <class T>
typename OrderedVector<T>::base_iterator& OrderedVector<T>::iterator::operator--()
{
    this->m_Iter --;
    return *this;
}

template <class T>
typename OrderedVector<T>::base_iterator OrderedVector<T>::iterator::operator--(int)
{
    base_iterator tmp(m_Iter --);
    return tmp;
}

template <class U>
std::ostream& operator<< (std::ostream &out, const OrderedVector<U> &vec)
{
    if (vec.m_shrPrtImpl != nullptr)
    {
        for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        {
            out << *it << "   ";
        }

        out << "size: " << vec.size() << "   ";
        out << "refs: " << vec.size_refs() << "   ";
        out << "capacity: " << vec.capacity() << "   ";
        out << "was changed: : " << (vec.is_dataChanged() ? "Yes" : "No");
        return out;
    }
    out << "Empty";
    return out;
}
#endif // OrderedVector_H

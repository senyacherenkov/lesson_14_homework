#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
#include <iterator>
#include <functional>
#include <iostream>

template <typename T, T DEFAULT_VALUE> class Element;
template <typename T, T DEFAULT_VALUE> std::ostream& operator<<( std::ostream&, const Element<T, DEFAULT_VALUE>& );

namespace  {
    using DestroyElementCallback = std::function<void(std::int32_t)>;
}

template<typename T, T DEFAULT_VALUE>
class Element {
    using MatrixCallback = std::function<void(std::int32_t, T)>;    
public:
    Element():
        m_callback(nullptr)
    {}

    Element(const Element& other) {
        m_number = other.m_number;
        m_baseElement = other.m_baseElement;
        m_callback = other.m_callback;
        m_destroyCallback = other.m_destroyCallback;
        if(other.m_baseElement == DEFAULT_VALUE)
            other.m_destroyCallback(other.m_number);
    }

    Element& operator= (const T& value) {
        m_baseElement = value;
        if((m_baseElement != DEFAULT_VALUE) && m_callback)
            m_callback(m_number, value);
        return *this;
    }

    bool operator== (const T& value) { return m_baseElement == value; }

    void setNumberKey(std::int32_t key) { m_number = key; }
    void setCallback(MatrixCallback callback) { m_callback = callback; }
    void setDestroyCallback(DestroyElementCallback callback) { m_destroyCallback = callback; }
    T getElement() const { return m_baseElement; }

private:
    std::size_t             m_number = 0;
    T                       m_baseElement;
    MatrixCallback          m_callback;
    DestroyElementCallback  m_destroyCallback;
};

template <typename T, T DEFAULT_VALUE>
class Row
{
    using MatrixCallback = std::function<void(std::int32_t, T)>;
public:
    Row():
        m_callback(nullptr),
        m_destroyCallBack(std::bind(&Row::destroyElement, this, std::placeholders::_1))
    {}

    auto& operator[] (const std::int32_t& column)
    {
        if(m_ElementBuff.find(column) == m_ElementBuff.end()) {
            m_ElementBuff[column] = DEFAULT_VALUE;
            if(m_callback)
                m_ElementBuff[column].setCallback(m_callback);
            m_ElementBuff[column].setNumberKey(column);
            m_ElementBuff[column].setDestroyCallback(m_destroyCallBack);
        }
        return m_ElementBuff[column];
    }

    void setCallback(MatrixCallback callback) { m_callback = callback; }
    void setDestroyCallback(DestroyElementCallback callback) { m_destroyMatrixCallBack = callback; }
    std::size_t size() { return m_ElementBuff.size(); }

    void destroyElement(std::int32_t key) {
        if(m_ElementBuff.find(key) != m_ElementBuff.end())
            m_ElementBuff.erase(key);
        if(m_destroyMatrixCallBack)
            m_destroyMatrixCallBack(key);
    }
private:
    std::map<std::int32_t, Element<T, DEFAULT_VALUE>>   m_ElementBuff;
    MatrixCallback                                      m_callback;
    DestroyElementCallback                              m_destroyCallBack;
    DestroyElementCallback                              m_destroyMatrixCallBack;
};


template <typename T, T DEFAULT_VALUE>
class Matrix
{
    using MatrixCallback = std::function<void(std::int32_t, T)>;
public:
    struct iterator_ {
        std::vector<std::tuple<int, int, T>> m_data;
        std::size_t m_pointer = 0;

        iterator_(std::vector<std::tuple<int, int, T>>& data):
            m_data(data),
            m_pointer(0)
        {}

        iterator_(std::vector<std::tuple<int, int, T>>& data, std::size_t pointer):
            m_data(data),
            m_pointer(pointer)
        {}

        bool operator!=(const iterator_& other) { return !(*this == other); }
        bool operator==(const iterator_& other) { return m_pointer == other.m_pointer; }

        iterator_& operator++() {
            m_pointer++;
            return *this;
        }

        iterator_ operator++(int) {
            iterator_ result(*this);
            ++(*this);
            return result;
        }
        std::tuple<int, int, T>& operator*() { return m_data.at(m_pointer); }
    };

public:
    Matrix():
        m_callBack(std::bind(&Matrix::fillMatrixStore, this, std::placeholders::_1, std::placeholders::_2)),
        m_destroyMatrixCallBack(std::bind(&Matrix::destroyRows, this, std::placeholders::_1))
    {}

    auto& operator[] (const std::int32_t& row)
    {
        m_row = row;
        auto& tempRow = m_rowBuff[row];
        tempRow.setCallback(m_callBack);
        tempRow.setDestroyCallback(m_destroyMatrixCallBack);
        return tempRow;
    }

    std::int32_t size() const { return m_store.size(); }

    void fillMatrixStore(std::int32_t key, T value) {
        m_store.push_back(std::make_tuple(m_row, key, value));
    }

    void destroyRows(std::int32_t key) {
        for(auto iter = m_rowBuff.begin(); iter != m_rowBuff.end(); )
        {
            if(iter->second.size() == 0)
            {
                m_rowBuff.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }
    iterator_ begin() { return iterator_(m_store); }
    iterator_ end() {return iterator_(m_store, m_store.size()); }

private:
    std::map<std::int32_t, Row<T,DEFAULT_VALUE>>    m_rowBuff;
    std::vector<std::tuple<int, int, T>>            m_store;    
    std::int32_t                                    m_row = 0;
    MatrixCallback                                  m_callBack;
    DestroyElementCallback                          m_destroyMatrixCallBack;
};

template <typename T, T DEFAULT_VALUE>
std::ostream& operator<<( std::ostream& os, const Element<T, DEFAULT_VALUE>& t ) {
   os << t.getElement();
   return os;
}

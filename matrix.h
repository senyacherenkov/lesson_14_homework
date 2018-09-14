#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>

#include <map>
#include <cassert>
#include <iterator>
#include <functional>

namespace  {
    using IncrementCallback = std::function<void()>;
}
template<typename T, T DEFAULT_VALUE>
class Element {

public:
    Element():
        m_callback(nullptr)
    {}

    Element& operator= (const T& value) {
        m_baseElement = value;
        if(m_baseElement != DEFAULT_VALUE && !m_callback)
            m_callback();
    }

    void setCallback(IncrementCallback callback) { m_callback = callback; }
private:
    T m_baseElement;
    IncrementCallback m_callback;
};

template <typename T, T DEFAULT_VALUE>
class Row
{
public:
    Row():
        m_callback(nullptr)
    {}

    T& operator[] (const T& column)
    {
        if(m_ElementBuff.find(column) == m_ElementBuff.end()) {
            m_ElementBuff[column] = DEFAULT_VALUE;
            if(!m_callback)
                m_ElementBuff[column].setCallback(m_callback);
        }
        return m_ElementBuff[column];
    }

    void setCallback(IncrementCallback callback) { m_callback = callback; }
private:
    std::map<std::int32_t, Element<T, DEFAULT_VALUE>> m_ElementBuff;
    IncrementCallback m_callback;
};


template <typename T, T DEFAULT_VALUE>
class Matrix
{
public:
    Matrix():
        m_incrementSize(incrementSize)
    {}

    auto& operator[] (const T& row)
    {
        auto& tempRow = m_RawBuff[row];
        tempRow.setCallback(m_incrementSize);
        return tempRow;
    }

    std::size_t size() const { return m_size; }

    void incrementSize() { m_size++;}

private:
    std::map<std::int32_t, Row<T,DEFAULT_VALUE>> m_RawBuff;
    std::size_t m_size;
    IncrementCallback m_incrementSize;
};

#pragma once
#include <unordered_map>
#include <cassert>
#include <memory>
#include <iostream>

template <typename T, T DEFAULT>
class Matrix
{
    using Index = std::int32_t;
    using Cell = std::pair<Index, Index>;

    //For more effective code I want to use std::unordered_map instead of std::map. As a key I want to use pair of values:
    //    - raw;
    //    - column.
    //Due to fact that we cannot calculate hash from std::pair we need to provide user vision of hash from std::pair.

    struct pairhash
    {
    public:
        //user hash should be a callable object
        std::size_t operator()(const Cell& key) const
        {
            return std::hash<Index>()(key.first) ^ std::hash<Index>()(key.second); //Internet sad, that it should be XOR
        }
    };

    using HashMap = std::unordered_map<Cell, T, pairhash>;
    using HashMapIterator = typename std::unordered_map<Cell, T, pairhash>::iterator;

    //--------------------------------------------------------------------------------------------------------------------
    class ColumnProxy
    {
    public:
        ColumnProxy(Matrix* matrix, Index raw, Index column)
            : m_matrix(matrix), m_row(raw), m_column(column) { }

        ColumnProxy& operator= (const T& value)
        {
            if(value != DEFAULT)
                m_matrix->insert(m_row, m_column, value);
            return *this;
        }

        operator T() const { return m_matrix->getValue(m_row, m_column); }
    private:
        Matrix* m_matrix;
        const Index m_row;
        const Index m_column;
    };

    //--------------------------------------------------------------------------------------------------------------------
    class RawProxy
    {
    public:
        RawProxy(Matrix* matrix, const Index row): m_matrix(matrix), m_row(row) {}
        auto operator[] (const Index column) { return ColumnProxy(m_matrix, m_row, column); }
    private:
        Matrix* m_matrix;
        const Index m_row;
    };

    //--------------------------------------------------------------------------------------------------------------------

    class MatrixIterator
    {
    public:
        MatrixIterator(const MatrixIterator &it) { m_iterator = it.m_iterator; }
        MatrixIterator(HashMapIterator _p) : m_iterator(_p) { }
        bool operator!=(MatrixIterator const& other) const { return m_iterator != other.m_iterator; }
        bool operator==(MatrixIterator const& other) const { return m_iterator == other.m_iterator; }
        auto operator*() const { return std::tuple<Index,Index,T>(m_iterator->first.first, m_iterator->first.second, m_iterator->second); }
        MatrixIterator& operator++()
        {
            ++m_iterator;
            return *this;
        }
    private:
        HashMapIterator m_iterator;
    };

public:
    Matrix() = default;

    auto operator[] (const Index row) { return RawProxy(this, row); }


    auto begin() { return MatrixIterator(m_data.begin()); }
    auto end() { return MatrixIterator(m_data.end()); }

    std::size_t size() const { return m_data.size(); }

private:
    void insert(Index row, Index column, const T& value) { m_data[Cell(row, column)] = value; }

    T getValue(Index row, Index column)
    {
        auto value = m_data.find(Cell(row, column));
        if (value != m_data.end())
            return value->second;
        else
            return DEFAULT;
    }
private:
    HashMap m_data;
};

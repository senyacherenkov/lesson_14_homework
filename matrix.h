#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>

template<typename T, int N>
class Matrix {
private:
    /*****************************************/
    //Vector for private needs - begin
    template<typename U, int Nv = INT_MAX>
    class Vector {
        struct iterator_ {
            std::vector<T>& m_data;
            std::size_t m_pointer = 0;

            iterator_(std::vector<T>& data):
                m_data(data),
                m_pointer(0)
            {}

            iterator_(std::vector<T>& data, std::size_t pointer):
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
            T& operator*() { return m_data.at(m_pointer); }
        };

    public:
        Vector() = default;

        iterator_ begin() { return iterator_(m_linedata); }
        iterator_ end() {return iterator_(m_linedata, m_linedata.size()); }

        T& operator [](int i) {
            if(i >= m_linedata.size()) {
                if(N == INT_MAX) //if it vector of vector - don't fill matrix with default value
                {
                    m_linedata.resize(i + 1);
                }
                else
                {
                    m_linedata.resize(i + 1, Nv);
                }
            }
            return m_linedata.at(i);
        }

        std::size_t size() const { return m_linedata.size();}
    private:
        std::vector<T> m_linedata;
    };
    //Vector for private needs - end
    /*****************************************/

    //Matrix iterator for cycles
    struct iterator_ {
        Vector<Vector<T, N>>    m_data;
        std::size_t             m_i = 0;
        std::size_t             m_j = 0;

        iterator_(Vector<Vector<T, N>>& data):
            m_data(data),
            m_i(0),
            m_j(0)
        {}

        iterator_(Vector<Vector<T, N>>& data, std::size_t i, std::size_t j):
            m_data(data),
            m_i(i),
            m_j(j)
        {}

        bool operator!=(const iterator_& other) { return !(*this == other); }
        bool operator==(const iterator_& other) { return ((m_i == other.m_i) && (m_j = other.m_j)); }

        iterator_& operator++() {
            bool found = false;
            for(const auto& vector: m_data)
            {
                for(const auto& element: vector)
                {
                    if(element != N)
                    {
                        found = true;
                        break;
                    }
                    m_j++;
                }
                if(!found)
                    m_i++;
            }
            return *this;
        }

        iterator_ operator++(int) {
            iterator_ result(*this);
            ++(*this);
            return result;
        }
        std::tuple<int, int, T> operator*() { return std::make_tuple(m_i, m_j, m_data[m_i][m_j]); }
    };

public:
    Matrix() = default;
    Vector<T, N>& operator [](int i)
    {
        return m_MatrixData[i];
    }

    iterator_ begin() { return iterator_(m_MatrixData); }
    iterator_ end() {return iterator_(m_MatrixData, m_MatrixData.size(), m_MatrixData[m_MatrixData.size() - 1].size()); }

    std::size_t size() {
        for(const auto& vect: m_MatrixData){
            for(const auto& element: vect){
                if(element != N)
                    m_size++;
            }
        }
        return m_size;
    }    
private:
    Vector<Vector<T, N>>    m_MatrixData;
    std::size_t             m_size;
};


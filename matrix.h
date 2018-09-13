#pragma once
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>

class Matrix;

template <typename T, typename U>
class Representation {
public:
    U default_value;
    Representation(Matrix& matrix):
        m_matrix(matrix)
    {}

    T& operator=(T value){
        if(value != default_value)
            notify();
        return m_value;
    }

    void notify();
private:
    T m_value = default_value;
    Matrix m_matrix;
};

template<typename T, typename U>
class Matrix {
private:

    Representation<T,N>& operator[](int n) { return m_representation;}
private:
    Representation<T,N>     m_representation;
    std::size_t             m_size;
};


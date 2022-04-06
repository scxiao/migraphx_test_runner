#ifndef __UTILITIES_HPP__
#define __UTILITIES_HPP__

#include <string>
#include <vector>
#include <ostream>
#include <iostream>

template<class T>
void print_vec(std::ostream& os, const std::vector<T>& vec, std::size_t column_size)
{
    os << "{";
    if (vec.size() <= 8 * column_size)
    {
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (i == vec.size() - 1) os << vec[i];
            else os << vec[i] << ", ";
            if ((i + 1) % column_size == 0)
            {
                os << std::endl;
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < 4 * column_size; ++i)
        {
            os << vec[i] << ", ";
            if ((i + 1) % column_size == 0)
            {
                os << std::endl;
            }
        }
        os << "..." << std::endl;
        std::size_t offset = vec.size() - 4 * column_size;
        for (std::size_t i = 0; i < 4 * column_size; ++i)
        {
            if (i == vec.size() - 1) os << vec[i + offset];
            else os << vec[i + offset] << ", ";
            if ((i + 1) % column_size == 0)
            {
                os << std::endl;
            }
        }
    }
    os << "}";
}

template<class T>
void print_vec(std::vector<T>& vec, std::size_t column_size)
{
    print_vec(std::cout, vec, column_size);
}


template<class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& vec)
{
    print_vec(os, vec, 8);
    return os;
}

#endif


#ifndef __RUN_TESTS_HPP__
#define __RUN_TESTS_HPP__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cmath>
#include <unordered_map>
#include <hip/hip_fp16.h>
#include "utilities.hpp"
#include <migraphx/migraphx.hpp>

template <class T>
auto get_hash(const T& x)
{
    return std::hash<T>{}(x);
}

std::string type_name(migraphx_shape_datatype_t type)
{
    std::unordered_map<migraphx_shape_datatype_t, std::string> type_name = 
    {
        {migraphx_shape_float_type, "float"},
        {migraphx_shape_half_type, "half"},
        {migraphx_shape_double_type, "double"},
        {migraphx_shape_int32_type, "int32_t"},
        {migraphx_shape_int64_type, "int64_t"},
        {migraphx_shape_int8_type, "int8_t"},
        {migraphx_shape_uint32_type, "uint32_t"},
        {migraphx_shape_uint64_type, "uint64_t"},
        {migraphx_shape_uint8_type, "uint8_t"},
        {migraphx_shape_bool_type, "bool"},
        {migraphx_shape_uint16_type, "uin16_t"},
        {migraphx_shape_int16_type, "int16_t"}
    };

    if (type_name.count(type) == 0)
    {
        std::cout << "Type " + std::to_string(type) + " does not exist!" << std::endl;
    }

    return type_name[type];
}

void print_argument(std::ostream& os, const migraphx::argument& arg)
{
    auto s = arg.get_shape();
    auto lens = s.lengths();
    auto elem_num = std::accumulate(lens.begin(), lens.end(), 1, std::multiplies<size_t>());
    migraphx_shape_datatype_t type = s.type();
    os << "type = " << type_name(type) << ", lens = " << s.lengths() << std::endl;
    if (type == migraphx_shape_float_type)
    {
        float *ptr = reinterpret_cast<float*>(arg.data());
        std::vector<float> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_half_type)
    {
        half *ptr = reinterpret_cast<half*>(arg.data());
        std::vector<half> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_double_type)
    {
        double *ptr = reinterpret_cast<double*>(arg.data());
        std::vector<double> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_int32_type)
    {
        int *ptr = reinterpret_cast<int*>(arg.data());
        std::vector<int> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_int64_type)
    {
        int64_t *ptr = reinterpret_cast<int64_t*>(arg.data());
        std::vector<int64_t> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_int8_type)
    {
        int8_t *ptr = reinterpret_cast<int8_t*>(arg.data());
        std::vector<int8_t> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_uint32_type)
    {
        uint32_t *ptr = reinterpret_cast<uint32_t*>(arg.data());
        std::vector<int32_t> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_uint64_type)
    {
        uint64_t *ptr = reinterpret_cast<uint64_t*>(arg.data());
        std::vector<uint64_t> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_uint8_type)
    {
        uint8_t *ptr = reinterpret_cast<uint8_t*>(arg.data());
        std::vector<uint8_t> data(ptr, ptr + elem_num);
        os << data;
    }
    else if (type == migraphx_shape_bool_type)
    {
        bool *ptr = reinterpret_cast<bool*>(arg.data());
        std::vector<bool> data(ptr, ptr + elem_num);
        os << data;
    }
    else
    {
        std::cout << "Type not support" << std::endl;
        std::abort();
    }
}

std::ostream& operator << (std::ostream& os, const migraphx::argument& arg)
{
    print_argument(os, arg);
    return os;
}

template <class T, class U>
void assign_value(const T* val, size_t num, std::vector<U>& output)
{
    for (size_t i = 0; i < num; ++i)
    {
        output.push_back(val[i]);
    }
}

template<class T>
void retrieve_argument_data(const migraphx::argument& argu, std::vector<T>& output)
{
    auto s = argu.get_shape();
    auto lens = s.lengths();
    auto elem_num = std::accumulate(lens.begin(), lens.end(), 1, std::multiplies<size_t>());
    migraphx_shape_datatype_t type = s.type();
    if (type == migraphx_shape_float_type)
    {
        float *ptr = reinterpret_cast<float*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_half_type)
    {
        half *ptr = reinterpret_cast<half*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_double_type)
    {
        double *ptr = reinterpret_cast<double*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_int32_type)
    {
        int *ptr = reinterpret_cast<int*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_int64_type)
    {
        int64_t *ptr = reinterpret_cast<int64_t*>(argu.data());
        std::cout << "val = " << *ptr << std::endl;
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_int8_type)
    {
        int8_t *ptr = reinterpret_cast<int8_t*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_uint32_type)
    {
        uint32_t *ptr = reinterpret_cast<uint32_t*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_uint64_type)
    {
        uint64_t *ptr = reinterpret_cast<uint64_t*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_uint8_type)
    {
        uint8_t *ptr = reinterpret_cast<uint8_t*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else if (type == migraphx_shape_bool_type)
    {
        bool *ptr = reinterpret_cast<bool*>(argu.data());
        assign_value(ptr, elem_num, output);
    }
    else
    {
        std::cout << "Type not support" << std::endl;
        std::abort();
    }
}

template<typename T>
bool compare_results(const T& gold, const T& actual, double eps)
{
    bool passed = true;
    std::size_t cpu_size = gold.size();
    float fmax_diff = 0.0f;
    size_t max_index = 0;
    for (std::size_t i = 0; i < cpu_size; i++) {
        auto diff = fabs(gold[i] - actual[i]);
        if (diff > eps)
        {
            if (fmax_diff < diff) 
            {
                fmax_diff = diff;
                max_index = i;
                passed = false;
            }
            std::cout << "gold[" << i << "] (" << gold[i] << ") != actual[" << i << "] (" <<
                actual[i] << ")!!!!!!" << std::endl;
        }
    }

    if (!passed)
    {
        size_t i = max_index;
        std::cout << "gold[" << i << "] (" << gold[i] << ") != actual[" << i << "] (" <<
            actual[i] << ")!!!!!!" << std::endl;

        std::cout << "max_diff = " << fmax_diff << std::endl;
    }

    return passed;
}

bool compare_results(const std::vector<int64_t>& gold, const std::vector<int64_t>& actual)
{
    bool passed = true;
    std::size_t cpu_size = gold.size();
    for (std::size_t i = 0; i < cpu_size; i++) {
        if (gold[i] - actual[i] != 0)
        {
            std::cout << "gold[" << i << "] (" << gold[i] << ") != actual[" << i << "] (" <<
                actual[i] << ")!!!!!!" << std::endl;
            passed = false;
        }
    }

    return passed;
}

bool compare_shapes(const migraphx::shape& s1, const migraphx::shape& s2)
{
    auto lens1 = s1.lengths();
    migraphx_shape_datatype_t type1 = s1.type();    
    auto lens2 = s2.lengths();
    migraphx_shape_datatype_t type2 = s2.type();
    if (type1 != type2)
    {
        std::cout << "Shape types are different!" << std::endl;
        return false;
    }

    if (lens1 != lens2)
    {
        std::cout << "Shape dims are different!" << std::endl;
        return false;
    }

    return true;
}

bool compare_results(const migraphx::argument& gold, const migraphx::argument& actual, double eps = 0.004)
{
    if (not compare_shapes(gold.get_shape(), actual.get_shape()))
    {
        return false;
    }

    std::cout << "gold = " << gold << std::endl;
    std::cout << "..." << std::endl;
    std::cout << "Actual = " << actual << std::endl;

    auto type = gold.get_shape().type();
    if (type == migraphx_shape_double_type or type == migraphx_shape_float_type or migraphx_shape_half_type)
    {
        std::vector<double> res1, res2;
        retrieve_argument_data(gold, res1);
        retrieve_argument_data(actual, res2);

        return compare_results(res1, res2, eps);
    }
    else
    {
        std::vector<int64_t> res1, res2;
        retrieve_argument_data(gold, res1);
        retrieve_argument_data(actual, res2);

        return compare_results(res1, res2);        
    }
}

#endif


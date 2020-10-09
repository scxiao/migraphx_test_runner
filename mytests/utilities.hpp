#ifndef _TEST_UTILITIES_HPP_
#define _TEST_UTILITIES_HPP_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <migraphx/program.hpp>
#include <migraphx/literal.hpp>
#include <migraphx/operators.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/instruction.hpp>
#include <migraphx/cpu/target.hpp>
#include <migraphx/gpu/target.hpp>
#include <migraphx/gpu/hip.hpp>
#include <migraphx/manage_ptr.hpp>
#include <migraphx/type_name.hpp>
#include "test.hpp"

template<typename T>
void print_res(const T& res)
{
    for (std::size_t i = 0; i < res.size(); ++i)
    {
        std::cout << std::setprecision(9) << std::setw(12) << res[i] << ", ";
        if ((i + 1) % 6 == 0) {
            std::cout << std::endl;
        }
    }
}

template <class T>
auto get_hash(const T& x)
{
    return std::hash<T>{}(x);
}

migraphx::argument gen_argument(migraphx::shape s, unsigned long seed)
{
    migraphx::argument result;
    s.visit_type([&](auto as) {
        using type = typename decltype(as)::type;
		std::vector<type> v(s.elements());
        std::srand(seed);
		for_each(v.begin(), v.end(), [&](auto val) { val = 1.0 * std::rand()/(RAND_MAX); } );
        //std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << std::endl;
        //result     = {s, [v]() mutable { return reinterpret_cast<char*>(v.data()); }};
    });

    return result;
}

template<class T>
void run_cpu(migraphx::program p, std::vector<T> &resData)
{
    p.compile(migraphx::cpu::target{});

    migraphx::program::parameter_map m;
    for (auto &&x : p.get_parameter_shapes())
    {
        //auto &&argu = gen_argument(x.second, get_hash(x.first));
        auto &&argu = migraphx::generate_argument(x.second, get_hash(x.first));
        m[x.first] = argu;
        //std::cout << "cpu_arg = " << argu << std::endl;
    }
    auto result = p.eval(m).back();
    //auto result = p.eval(m);
    result.visit([&](auto output) { resData.assign(output.begin(), output.end()); });

    std::cout << "cpu output_shape = " << result.get_shape() << std::endl;
    std::cout << "cpu res = " << std::endl;
    print_res(resData);
    std::cout << std::endl;
}

template <class T>
void run_gpu(migraphx::program p, std::vector<T> &resData)
{
    p.compile(migraphx::gpu::target{});

    migraphx::program::parameter_map m;
    for (auto &&x : p.get_parameter_shapes())
    {
        std::cout << "gpu input: " << x.first << "\'shape = " << x.second << std::endl;
        //auto&& argu = gen_argument(x.second, get_hash(x.first));
        auto&& argu = migraphx::generate_argument(x.second, get_hash(x.first));
        //std::cout << "gpu_arg = " << argu << std::endl;
        m[x.first] = migraphx::gpu::to_gpu(argu);
    }

    auto result = migraphx::gpu::from_gpu(p.eval(m).back());
    result.visit([&](auto output) { resData.assign(output.begin(), output.end()); });

    //migraphx::gpu::from_gpu(p.eval(m));
    //auto result = migraphx::gpu::from_gpu(m["output"]);
    result.visit([&](auto output) { resData.assign(output.begin(), output.end()); });


    std::cout << "gpu output_shape = " << result.get_shape() << std::endl;
    std::cout << "gpu res = " << std::endl;
    print_res(resData);
    std::cout << std::endl;
}


void print_vec(std::vector<float>& vec, std::size_t column_size)
{
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << "\t";
        if ((i + 1) % column_size == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

migraphx::program::parameter_map create_param_map(migraphx::program& p)
{
    migraphx::program::parameter_map m;
    for (auto&& x : p.get_parameter_shapes())
    {
        if (x.second.type() == migraphx::shape::int32_type or
            x.second.type() == migraphx::shape::int64_type)
        {
            auto&& argu = migraphx::fill_argument(x.second, 0);
            m[x.first] = argu;
        }
        else
        {
            m[x.first] = migraphx::generate_argument(x.second, get_hash(x.first));
        }
    }

    return m;
}

template <class T>
void run_prog(migraphx::program p, const migraphx::target& t, std::vector<std::vector<T>> &resData)
{
    p.compile(t);
    std::cout << "compiled program = " << std::endl;
    std::cout << p << std::endl;
    std::string print_name = t.name();
    if (print_name == "miopen")
    {
        print_name = "gpu";
    }
    std::cout << "run on " << print_name << "............." << std::endl << std::endl;

    migraphx::program::parameter_map m;
    std::vector<int64_t> lens = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> indices = {2, 1, 2, 0, 1, 0};
    for (auto &&x : p.get_parameter_shapes())
    {
        std::cout << "input: " << x.first << "\'shape = " << x.second << std::endl;
        if (x.first == "lengths")
        {
            auto&& argu = migraphx::argument(x.second, lens.data());
            m[x.first] = t.copy_to(argu);
        }
        else if (x.first == "indices")
        {
            auto&& argu = migraphx::argument(x.second, indices.data());
            //std::cout << "argu = " << argu << std::endl;
            m[x.first] = t.copy_to(argu);
        }
        else if (x.second.type() == migraphx::shape::int32_type or
            x.second.type() == migraphx::shape::int64_type)
        {
            auto&& argu = migraphx::fill_argument(x.second, 1);
            m[x.first] = t.copy_to(argu);
        }
        else
        {
            //auto&& argu = gen_argument(x.second, get_hash(x.first));
            auto&& argu = migraphx::generate_argument(x.second, get_hash(x.first));
            std::cout << "argu = " << argu << std::endl;
            std::vector<float> vec_arg;
            argu.visit([&](auto v) { vec_arg.assign(v.begin(), v.end()); });
            m[x.first] = t.copy_to(argu);
        }
    }

    std::cout << "Begin execution ...." << std::endl;
    auto results = p.eval(m);
    std::cout << "End execution ...." << std::endl;

    std::size_t i = 0;
    for (auto&& result : results)
    {
        auto cpu_res = t.copy_from(result);
        std::vector<T> resTmp;
        cpu_res.visit([&](auto output) { resTmp.assign(output.begin(), output.end()); });
        std::cout << "Output_" << i << "_shape = " << cpu_res.get_shape() << std::endl;
        std::cout << "Result_" << i << " = " << std::endl;
        resData.push_back(resTmp);
        print_res(resTmp);
        
        std::cout << std::endl;
        ++i;
    }
}


template<typename T>
bool compare_results(const T& cpu_res, const T& gpu_res)
{
    bool passed = true;
    std::size_t cpu_size = cpu_res.size();
    float fmax_diff = 0.0f;
    size_t max_index = 0;
    for (std::size_t i = 0; i < cpu_size; i++) {
        auto diff = fabs(cpu_res[i] - gpu_res[i]);
        if (diff > 1.0e-3)
        {
            if (fmax_diff < diff) 
            {
                fmax_diff = diff;
                max_index = i;
                passed = false;
            }
            std::cout << "cpu_result[" << i << "] (" << cpu_res[i] << ") != gpu_result[" << i << "] (" <<
                gpu_res[i] << ")!!!!!!" << std::endl;
        }
    }

    if (!passed)
    {
        size_t i = max_index;
        std::cout << "cpu_result[" << i << "] (" << cpu_res[i] << ") != gpu_result[" << i << "] (" <<
            gpu_res[i] << ")!!!!!!" << std::endl;

        std::cout << "max_diff = " << fmax_diff << std::endl;
    }

    return passed;
}

bool compare_results(const std::vector<int>&cpu_res, const std::vector<int>& gpu_res)
{
    bool passed = true;
    std::size_t cpu_size = cpu_res.size();
    for (std::size_t i = 0; i < cpu_size; i++) {
        if (cpu_res[i] - gpu_res[i] != 0)
        {
            std::cout << "cpu_result[" << i << "] (" << cpu_res[i] << ") != gpu_result[" << i << "] (" <<
                gpu_res[i] << ")!!!!!!" << std::endl;
            passed = false;
        }
    }

    return passed;
}

bool compare_results(const std::vector<int64_t>&cpu_res, const std::vector<int64_t>& gpu_res)
{
    bool passed = true;
    std::size_t cpu_size = cpu_res.size();
    for (std::size_t i = 0; i < cpu_size; i++) {
        if (cpu_res[i] - gpu_res[i] != 0)
        {
            std::cout << "cpu_result[" << i << "] (" << cpu_res[i] << ") != gpu_result[" << i << "] (" <<
                gpu_res[i] << ")!!!!!!" << std::endl;
            passed = false;
        }
    }

    return passed;
}


template<typename T>
std::vector<T> read_input(std::string file_name)
{
    std::vector<T> res;
    std::ifstream ifs(file_name);
    if (!ifs.is_open())
    {
        return {};
    }

    int num;
    while (ifs >> num)
    {
        res.push_back(static_cast<T>(num));
    }
    ifs.close();

    return res;
}


#endif


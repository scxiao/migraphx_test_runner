#include <iostream>
#include <vector>
#include <string>
#include <migraphx/literal.hpp>
#include <migraphx/quantization.hpp>
#include <migraphx/operators.hpp>
#include <migraphx/program.hpp>
#include <migraphx/instruction.hpp>
#include <migraphx/gpu/target.hpp>
#include <migraphx/cpu/target.hpp>
#include <migraphx/onnx.hpp>

void load_onnx_file(std::string file_name) {
    auto prog = migraphx::parse_onnx(file_name);
    std::cout << "Load program is: " << std::endl;
    std::cout << prog << std::endl;
    //migraphx::capture_arguments(prog, migraphx::cpu::target{});
    //std::vector<std::string> op_names = {"convolution", "dot"};
    //std::vector<std::pair<float, float>> quant_params(300, std::make_pair<float, float>(1.0f, 0.0f));
    //migraphx::quantize_int8(prog, op_names, quant_params);
    //std::cout << "Quantized program is: " << std::endl;
    //std::cout << prog << std::endl;
    prog.compile(migraphx::gpu::target{});
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " onnx_file" << std::endl;
        return 0;
    }

    load_onnx_file(argv[1]);

    return 0;
}


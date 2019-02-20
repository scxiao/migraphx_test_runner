#include <iostream>
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

template <class T>
auto get_hash(const T& x)
{
    return std::hash<T>{}(x);
}

int main()
{
    {
		std::size_t batch_size  = 3;
		std::size_t seq_len     = 4;
		std::size_t hidden_size = 4;
		std::size_t input_size  = 3;
		std::size_t num_dirct   = 1;
		std::vector<float> w_data{
		   0.1236, -0.3942,  0.4149,
		   0.0795,  0.4934, -0.2858,
		   0.2602, -0.3098,  0.0567,
		   0.3344,  0.3607, -0.0551,
		   0.4952,  0.3799,  0.0630,
		  -0.3532,  0.0023, -0.0592,
		   0.4267,  0.2382, -0.0784,
		  -0.0032, -0.2476, -0.0206,
		  -0.4963,  0.4837,  0.0827,
		   0.0123, -0.1203, -0.0279,
		  -0.0049,  0.4721, -0.3564,
		  -0.1286,  0.4090, -0.0504,
		   0.0575, -0.2138,  0.1071,
		   0.1976, -0.0758,  0.0139,
		  -0.0761,  0.3991, -0.2965,
		  -0.4845, -0.1496,  0.3285};

		std::vector<float> r_data{
		   0.1237,  0.1229, -0.0766, -0.1144,
		  -0.1186,  0.2922,  0.2478,  0.3159,
		  -0.0522,  0.1685, -0.4621,  0.1728,
		   0.0670, -0.2458, -0.3835, -0.4589,
		  -0.3109,  0.4908, -0.0133, -0.1858,
		  -0.0590, -0.0347, -0.2353, -0.0671,
		  -0.3812, -0.0004, -0.1432,  0.2406,
		   0.1033, -0.0265, -0.3902,  0.0755,
		   0.3733,  0.4383, -0.3140,  0.2537,
		  -0.1818, -0.4127,  0.3506,  0.2562,
		   0.2926,  0.1620, -0.4849, -0.4861,
		   0.4426,  0.2106, -0.0005,  0.4418,
		  -0.2926, -0.3100,  0.1500, -0.0362,
		  -0.3801, -0.0065, -0.0631,  0.1277,
		   0.2315,  0.4087, -0.3963, -0.4161,
		  -0.2169, -0.1344,  0.3468, -0.2260};

		std::vector<float> bias_data{
		   0.0088,  0.1183,  0.1642, -0.2631, -0.1330, -0.4008,  0.3881, -0.4407, -0.2760,
		   0.1274, -0.0083, -0.2885,  0.3949, -0.0182,  0.4445,  0.3477,  0.2266,  0.3423,
		  -0.0674, -0.4067,  0.0807,  0.1109, -0.2036,  0.1782, -0.2467, -0.0730, -0.4216,
		   0.0316, -0.3025,  0.3637, -0.3181, -0.4655};

		std::vector<float> input_data{
			-0.5516,  0.2391, -1.6951,
			-0.4313, -0.9730, -0.2005,
			 2.3930, -0.5221, -0.1331,
			-0.0910,  1.2122, -0.1952,
			 0.4661,  0.6494,  2.1332,
			-1.0972,  0.9816,  0.1122,
			 0.3577,  1.3508, -0.5366,
			 1.7449,  0.5483, -0.0701,
			-0.4100, -2.2344,  0.3685,
			 0.4583,  2.3794,  1.0372,
			-0.8887,  0.7892, -0.4012,
			-0.2818, -2.3374,  1.5310};
            // 0.9474, -0.8781,  0.7545,
         	// 2.2865,  0.3695, -0.2122,
          	//-0.3526,  0.8882, -0.6883};


        std::vector<float> ih_data{
		   1.9104, -1.9004,  0.3337,  0.5741,
		   0.5671,  0.0458,  0.4514, -0.8968,
		  -0.9201,  0.1962,  0.5771, -0.5332};

		std::vector<float> ic_data{
		   0.9569, -0.5981,  1.1312,  1.0945,
		   1.1055, -0.1212, -0.9097,  0.7831,
		  -1.6991, -1.9498, -1.2567, -0.4114};

        std::vector<float> pph_data{
            1.84369764,  0.68413646, -0.44892886, -1.50904413,  
            0.3860796,  -0.52186625, 1.08474445, -1.80867321,  
            1.32594529,  0.4336262,  -0.83699064,  0.49162736};

		float clip          = 0.0f;
		{
			migraphx::program p;
			migraphx::shape in_shape{migraphx::shape::float_type, {seq_len, batch_size, input_size}};
			auto seq = p.add_literal(migraphx::literal{in_shape, input_data});

			migraphx::shape ih_shape{migraphx::shape::float_type, {num_dirct, batch_size, hidden_size}};
			auto ih = p.add_literal(migraphx::literal{ih_shape, ih_data});

			migraphx::shape ic_shape{migraphx::shape::float_type, {num_dirct, batch_size, hidden_size}};
			auto ic = p.add_literal(migraphx::literal{ic_shape, ic_data});

			migraphx::shape w_shape{migraphx::shape::float_type, {num_dirct, 4*hidden_size, input_size}};
			auto w = p.add_literal(migraphx::literal{w_shape, w_data});

			migraphx::shape r_shape{migraphx::shape::float_type, {num_dirct, 4*hidden_size, hidden_size}};
			auto r = p.add_literal(migraphx::literal{r_shape, r_data});

			migraphx::shape b_shape{migraphx::shape::float_type, {num_dirct, 8*hidden_size}};
			auto bias = p.add_literal(migraphx::literal{b_shape, bias_data});

            migraphx::shape pph_shape{migraphx::shape::float_type, {num_dirct, 3*hidden_size}};
            auto pph = p.add_literal(migraphx::literal{pph_shape, pph_data});
            auto und = p.add_instruction(migraphx::op::undefined{});

			auto hs = p.add_instruction(migraphx::op::lstm{hidden_size,
												{migraphx::op::sigmoid{}, migraphx::op::tanh{}, migraphx::op::tanh{}},
												migraphx::op::rnn_direction::forward,
												clip,
                                                0},
							  seq,
							  w,
							  r,
							  bias,
                              und,
                              ih,
							  ic,
							  pph);
            //p.add_instruction(migraphx::op::lstm_last_cell_output{}, hs);
			p.compile(migraphx::cpu::target{});
            migraphx::program::parameter_map m;
            for (auto &&x : p.get_parameter_shapes())
            {
                auto&& argu = migraphx::generate_argument(x.second, get_hash(x.first));
                m[x.first] = argu;
            }
 
            auto hs_concat = p.eval({m});
			std::vector<float> hs_data;
			hs_concat.visit([&](auto output) { hs_data.assign(output.begin(), output.end()); });

            for (std::size_t i = 0; i < hs_data.size(); ++i)
            {
                std::cout << std::setw(12) << hs_data.at(i) << ",";
                if ((i + 1) % hidden_size == 0)
                    std::cout << std::endl;
            }
		}
    }

        
    return 0;
}


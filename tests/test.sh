#!/bin/bash
now=$(date +"%Y_%m_%d_%H_%M_%S")
list_file=/home/scxiao/Workplace/tests/msft/models/list_migraphx
./onnx_test.sh ${list_file} migraphx 1 |& tee migraphx_1_$now
#./onnx_test.sh msft_models/list_migraphx migraphx 101 |& tee migraphx_101_$now


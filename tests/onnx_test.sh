#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: onnx_test.sh list_file ep_name iters"
    exit 0
fi

list_file=$1
ep_name=$2
iter=$3

echo "list file: $list_file"
echo "Run on $ep_name ...."

#model_loc="/code/onnxruntime/test/msft_models"
model_loc="/home/scxiao/Workplace/tests/msft/models"
model_num=0
while IFS= read -r line
do
  if [ -z "$line" ]; then
    continue
  fi

  IFS=' ' read -r -a array <<< "$line"
  if [ -z ${array[1]} ]; then
    array[1]=$3
  fi

  model_folder=${model_loc}${array[0]}

  echo "mode ${model_num}...."
  cmd="python3 test_runner.py ${model_folder}"
  echo "$cmd"
  time $cmd
  ((model_num=model_num+1))
  sleep 5
done < "$list_file"


# migraphx_test_runner
C++ and Python version of the migraphx_test_runner to run 
models with inputs and outputs to verify correctness. The 
[Python version](https://github.com/ROCmSoftwarePlatform/AMDMIGraphX/blob/develop/tools/test_runner.py)
was already merged to [AMDMIGraphX](https://github.com/ROCmSoftwarePlatform/AMDMIGraphX).

You can use the following steps to run the test runner:

## Build AMDMIGraphX
You can following the [steps](https://github.com/ROCmSoftwarePlatform/AMDMIGraphX#use-the-rocm-build-tool-rbuild) 
of building AMDMIGraphX from source using the rbuild.

For example, you can use the following command to build the source:

```
rbuild build -d deps -B build --cxx=/opt/rocm/llvm/bin/clang++
```

Note: the names `deps` and `build` should be used in the above command since the C++ version of 
test runner hard-coded these names in the file CMakeLists.txt.

## Python version command line
The command line of the Python version test runner is:

1) set the `PYTHONPATH1`
```
export PYTHONPATH=$PYTHONPATH:$path/AMDMIGraphX/build/lib
```

2) run the test runner

```
python3 test_runner.py path/to/example
```

The test example here uses the standard test examples with inputs and inputs in protobuf format. You can download some
example using the [script](https://github.com/ROCmSoftwarePlatform/AMDMIGraphX/blob/develop/tools/download_models.sh).


## C++ version command

The C++ version of test runner can be build as:

```
cmake .. -DCMAKE_PREFIX_PATH=${path}/AMDMIGraphX
```

where `${path}/AMDMIGraphX` is the MIGraphX repo folder.

Then the test runner can be executed as:

```
./migraphx_test_runner path/to/example
```

By default, the `gpu` target is used, you can set the target to `ref` with the option `-t ref`.



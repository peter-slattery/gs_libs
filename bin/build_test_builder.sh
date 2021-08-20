#!/bin/bash

pushd ../
mkdir tests_run_tree
pushd tests_run_tree

clang ../test_builder_src/first.c -o test_builder
chmod +x test_builder 

popd
popd
#!/bin/bash

mkdir ../tests_run_tree
pushd ../tests_run_tree

./test_builder /Users/jrinker/_ps/projects/gs_libs/tests_src/
chmod +x build_and_run_tests.sh
./build_and_run_tests.sh

popd

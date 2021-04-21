#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../../../planners/python"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DIR}/../../../../planners/python:${DIR}/../../../../planners/cpp:${DIR}/../../../../planners/java/JPlanner/grpc_cpp_client_test"

python3 ${DIR}/pyclient.py

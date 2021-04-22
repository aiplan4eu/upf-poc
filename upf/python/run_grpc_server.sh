#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../planners/python:${DIR}/../../planners/python/py_upf_wrapper:${DIR}/../../planners/cpp/py_upf_wrapper:${DIR}/../../planners/java/grpc_pyclient"

python3 ${DIR}/grpc/grpc_server.py

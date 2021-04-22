#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../planners/python:${DIR}/../../planners/cpp:${DIR}/../../planners/java/JPlanner/grpc_pyclient"

python3 ${DIR}/grpc/grpc_server.py
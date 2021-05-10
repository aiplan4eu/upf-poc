#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../planners/python:${DIR}/../../planners/python/py_upf_wrapper:${DIR}/../../planners/cpp/py_upf_wrapper:${DIR}/../../planners/java/grpc_pyclient:${DIR}/../../planners/java/py_upf_wrapper_javacpp"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DIR}/../../planners/java/py_upf_wrapper_javacpp/linux-x86_64:${DIR}/../../planners/java/py_upf_wrapper_javacpp"
export CLASSPATH="${CLASSPATH}:${DIR}/../../planners/java/py_upf_wrapper_javacpp:${DIR}/../../planners/java/py_upf_wrapper_javacpp/javacpp.jar:${DIR}/../../planners/java/JPlanner/build/classes/java/main"

python3 ${DIR}/grpc/grpc_server.py

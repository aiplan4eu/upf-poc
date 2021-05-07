#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../..:${DIR}/../../../../../planners/python:${DIR}/../../../../../planners/python/py_upf_wrapper:${DIR}/../../../../../planners/cpp/py_upf_wrapper:${DIR}/../../../../../planners/java/grpc_pyclient:${DIR}/../../../../../planners/java/py_upf_wrapper_javacpp"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DIR}/linux-x86_64:${DIR}/../../../../../planners/java/py_upf_wrapper_javacpp/linux-x86_64:${DIR}/../../../../../planners/java/py_upf_wrapper_javacpp"
export CLASSPATH="${CLASSPATH}:${DIR}/../../../../../planners/java/py_upf_wrapper_javacpp:${DIR}/../../../../../planners/java/py_upf_wrapper_javacpp/javacpp.jar:${DIR}/../../../../../planners/java/JPlanner/build/classes/java/main"


LD_PRELOAD=${DIR}/linux-x86_64/libjniUPF.so java -cp ./:javacpp.jar:../../../../../planners/java/py_upf_wrapper_javacpp:../../../../../planners/java/py_upf_wrapper_javacpp/linux-x86_64:../../../../../planners/java/JPlanner/build/classes/java/main Main
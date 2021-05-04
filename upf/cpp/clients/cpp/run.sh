#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../../../../planners/python:${DIR}/../../../../planners/python/cpp_upf_wrapper"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DIR}/../../../../planners/python/cpp_upf_wrapper:${DIR}/../../../../planners/cpp/cpp_upf_wrapper:${DIR}/../../../../planners/java/grpc_cpp_client:${DIR}/../../../../planners/java/cpp_upf_wrapper:${DIR}/../../../../planners/java/cpp_upf_wrapper/linux-x86_64"
export CLASSPATH="${CLASSPATH}:${DIR}/../../../../planners/java/cpp_upf_wrapper:${DIR}/../../../../planners/java/cpp_upf_wrapper/javacpp.jar:${DIR}/../../../../planners/java/JPlanner/build/classes/java/main"

${DIR}/cppclient

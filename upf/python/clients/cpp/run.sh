#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../..:${DIR}/../../../../planners/python:${DIR}/../../../../planners/python/py_upf_wrapper:${DIR}/../../../../planners/cpp/py_upf_wrapper:${DIR}/../../../../planners/java/py_upf_wrapper"

${DIR}/cppclient

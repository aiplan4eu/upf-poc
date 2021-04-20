#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

export PYTHONPATH="${PYTHONPATH}:${DIR}/../..:${DIR}/../../../../planners/python:${DIR}/../../../../planners/cpp:${DIR}/../../../../planners/java/JPlanner/grpc_client_test"

./cppclient

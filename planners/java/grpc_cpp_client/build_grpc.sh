#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

protoc -I ${DIR}/../JPlanner/src/main/proto --grpc_out=${DIR}/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ${DIR}/../JPlanner/src/main/proto/JPlannerUPF.proto
protoc -I ${DIR}/../JPlanner/src/main/proto --cpp_out=${DIR}/ ${DIR}/../JPlanner/src/main/proto/JPlannerUPF.proto

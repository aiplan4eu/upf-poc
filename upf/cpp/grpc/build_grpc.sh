#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

protoc -I ${DIR}/ --grpc_out=${DIR}/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ${DIR}/UPF.proto
protoc -I ${DIR}/ --cpp_out=${DIR}/ ${DIR}/UPF.proto

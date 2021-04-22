#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

python3 -m grpc_tools.protoc -I${DIR}/../src/main/proto --python_out=${DIR}/ --grpc_python_out=${DIR}/ ${DIR}/../src/main/proto/JPlannerUPF.proto

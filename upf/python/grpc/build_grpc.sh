#!/bin/sh

python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. ./UPF.proto

#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

bash ${DIR}/build_grpc.sh

export PKG_CONFIG_PATH="${HOME}/.local/lib/pkgconfig"

g++ -O3 -c -fPIC -Wall -Werror -std=c++17 -I${HOME}/.local/include ${DIR}/UPF.grpc.pb.cc -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl -o ${DIR}/UPF.grpc.pb.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 -I${HOME}/.local/include ${DIR}/UPF.pb.cc -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl -o ${DIR}/UPF.pb.o

g++ -O3 -fPIC -Wall -std=c++17 -I${HOME}/.local/include -I${DIR}/ -I${DIR}/.. ${DIR}/grpc_server.cxx ${DIR}/UPF.grpc.pb.o ${DIR}/UPF.pb.o ${DIR}/../upf.so -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl -o ${DIR}/grpc_server

rm ${DIR}/*.o

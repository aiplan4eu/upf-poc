#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

bash ${DIR}/build_grpc.sh

export PKG_CONFIG_PATH="${HOME}/.local/lib/pkgconfig"

g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../../../upf/cpp/action.cxx -o ${DIR}/action.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../../../upf/cpp/problem.cxx -o ${DIR}/problem.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 -I${HOME}/.local/include ${DIR}/JPlannerUPF.grpc.pb.cc -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl -o ${DIR}/JPlannerUPF.grpc.pb.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 -I${HOME}/.local/include ${DIR}/JPlannerUPF.pb.cc -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -ldl -o ${DIR}/JPlannerUPF.pb.o

g++ -O3 -shared -fPIC -Wall -std=c++17 -I${HOME}/.local/include -I${DIR}/ -I${DIR}/../../../../upf/cpp ${DIR}/jplanner_upf.cxx ${DIR}/JPlannerUPF.grpc.pb.o ${DIR}/JPlannerUPF.pb.o ${DIR}/action.o ${DIR}/problem.o -L${HOME}/.local/lib `pkg-config --libs protobuf grpc++` -lgrpc++_reflection -lpthread -ldl -o ${DIR}/jplanner_cppupf.so

rm ${DIR}/*.o

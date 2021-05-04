#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/action.cxx -o ${DIR}/action.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/problem.cxx -o ${DIR}/problem.o
g++ -O3 -shared -fPIC -Wall -Werror -std=c++17 -I${DIR}/ ${DIR}/upf.cxx ${DIR}/problem.o ${DIR}/action.o -o ${DIR}/upf.so
cp upf.so libupf.so

rm ${DIR}/*.o

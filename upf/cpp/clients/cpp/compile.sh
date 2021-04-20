#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -fPIC -Wall -Werror -std=c++17 -I${DIR}/../../ ${DIR}/main.cxx ${DIR}/../../upf.so -ldl -o ${DIR}/cppclient

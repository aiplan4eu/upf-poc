#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`
g++ -O3 -fPIC -Wall -std=c++17 -I${DIR}/../../../../pybind11/include -I/usr/include/python${PYTHON_VERSION} ${DIR}/main.cxx -lpython${PYTHON_VERSION} -o ${DIR}/cppclient

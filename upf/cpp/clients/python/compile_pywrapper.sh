#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`
g++ -O3 -Wall -shared -std=c++17 -fPIC -I${DIR}/../../ -I${DIR}/../../../../pybind11/include ${DIR}/upf_wrapper.cxx -I/usr/include/python${PYTHON_VERSION} ${DIR}/../../upf.so -o ${DIR}/_pywrapper_upf.so

#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -c -fPIC ${DIR}/planner.cxx -Wall -Werror -std=c++17 -o ${DIR}/planner.o

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`
g++ -O3 -Wall -shared -std=c++17 -fPIC -I${DIR}/../../pybind11/include ${DIR}/cppwrapper.cxx -I/usr/include/python${PYTHON_VERSION} ${DIR}/planner.o -o ${DIR}/_cppplanner_upf.so

rm ${DIR}/*.o

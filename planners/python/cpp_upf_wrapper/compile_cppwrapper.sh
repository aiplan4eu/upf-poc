#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../../upf/cpp/action.cxx -o ${DIR}/action.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../../upf/cpp/problem.cxx -o ${DIR}/problem.o

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`
g++ -O3 -shared -fPIC -Wall -Werror -std=c++17 -I${DIR}/../../../pybind11/include -I/usr/include/python${PYTHON_VERSION} -I${DIR}/../../../upf/cpp ${DIR}/pyplanner_upf.cxx ${DIR}/action.o ${DIR}/problem.o -lpython${PYTHON_VERSION} -o ${DIR}/pyplanner_upf.so

rm ${DIR}/*.o

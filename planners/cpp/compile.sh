#!/bin/sh

g++ -c -fPIC planner.cxx -Wall -Werror -std=c++17 -o planner.o
g++ planner.o main.cxx -Wall -Werror -std=c++17 -o planner

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`
g++ -Wall -shared -std=c++17 -fPIC -Ipybind11/include cppwrapper.cxx -I/usr/include/python${PYTHON_VERSION} planner.o -o cppplanner_upf.so
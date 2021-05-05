#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

java -jar javacpp.jar -header -classpath ../JPlanner/build/classes/java/main/:. PlannerWrapper.java

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`

g++ -march=x86-64 -m64 -O3 -Wl,-Bsymbolic -fPIC -shared -Wall -Wextra -std=c++17 -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/ -I${DIR}/../../../pybind11/include -I/usr/include/python${PYTHON_VERSION} jplanner_upf.cxx -L${DIR}/linux-x86_64 -ljniPlannerWrapper -o _jplanner_upf.so

#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/planner.cxx -o ${DIR}/planner.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../upf/cpp/action.cxx -o ${DIR}/action.o
g++ -O3 -c -fPIC -Wall -Werror -std=c++17 ${DIR}/../../upf/cpp/problem.cxx -o ${DIR}/problem.o
g++ -O3 -shared -fPIC -Wall -Werror -std=c++17 -I${DIR}/../../upf/cpp ${DIR}/cppplanner_upf.cxx ${DIR}/planner.o ${DIR}/action.o ${DIR}/problem.o -o ${DIR}/cppplanner_cppupf.so

rm ${DIR}/*.o

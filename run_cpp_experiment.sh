#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"


echo "--------------------------------------------------------------------------------"
echo "|  C++ UPF                                                                     |"
echo "--------------------------------------------------------------------------------"

echo " -> Launching Java GRPC Planning Server..."
cd ${DIR}/planners/java/JPlanner
./gradlew build &> /dev/null
./gradlew runServer &> /dev/null &
JAVA_SERVER_PID=$!
sleep 5
cd ${DIR}

cd ${DIR}/upf/cpp
echo " -> Compiling the UPF..."
bash compile_upf.sh &> /dev/null
echo "    Done."

echo " -> Compiling Planner Wrappers..."
echo "    - C++"
bash ${DIR}/planners/cpp/cpp_upf_wrapper/compile_cppwrapper.sh
echo "    - Python"
bash ${DIR}/planners/python/cpp_upf_wrapper/compile_cppwrapper.sh
echo "    - Java"
bash ${DIR}/planners/java/grpc_cpp_client/build_grpc.sh
bash ${DIR}/planners/java/grpc_cpp_client/compile_cppwrapper.sh
cd ${DIR}/planners/java/cpp_upf_wrapper
bash compile.sh &> /dev/null
cd ${DIR}
echo "    Done."

echo " -> Launching C++ UPF Server..."
cd ${DIR}/upf/cpp/grpc
echo " -> Compiling..."
bash compile_grpc_server.sh &> /dev/null
echo "    Done."
cd ..
bash run_grpc_server.sh &
UPF_SERVER_PID=$!
sleep 1
cd ${DIR}

echo ""
echo "TSB written in Python"
echo "---------------------"
bash upf/cpp/clients/python/compile_pywrapper.sh
bash upf/cpp/clients/python/run.sh

echo ""
echo "TSB written in Java via GRPC"
echo "-------------------"
cd ${DIR}/upf/cpp/clients/java/JClient
./gradlew --console plain -q runClient
cd ${DIR}

echo ""
echo "TSB written in Java"
echo "-------------------"
cd ${DIR}/upf/cpp/clients/java/JavacppClient
echo " -> Compiling..."
bash compile_jupf.sh &> /dev/null
echo "    Done."
bash run.sh
cd ${DIR}

echo ""
echo "TSB written in C++"
echo "------------------"
echo " -> Compiling..."
cd ${DIR}/upf/cpp/clients/cpp
bash compile.sh &> /dev/null
echo "    Done."
bash run.sh
cd ${DIR}

pkill -P ${UPF_SERVER_PID}; kill ${UPF_SERVER_PID}
kill -9 ${JAVA_SERVER_PID}

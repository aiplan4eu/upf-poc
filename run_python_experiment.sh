#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"


echo "--------------------------------------------------------------------------------"
echo "|  Python UPF                                                                  |"
echo "--------------------------------------------------------------------------------"

echo " -> Launching Java GRPC Planning Server..."
cd ${DIR}/planners/java/JPlanner
./gradlew runServer &> /dev/null &
JAVA_SERVER_PID=$!
sleep 5
cd ${DIR}

echo " -> Launching Python UPF Server..."
cd ${DIR}/upf/python/grpc
bash build_grpc.sh
cd ..
bash run_grpc_server.sh &
UPF_SERVER_PID=$!
sleep 1
cd ${DIR}

echo " -> Compiling Planner Wrappers..."
echo "    - C++"
bash ${DIR}/planners/cpp/py_upf_wrapper/compile_pywrapper.sh
echo "    - Java"
bash ${DIR}/planners/java/grpc_pyclient/build_grpc.sh
cd ${DIR}/planners/java/py_upf_wrapper_javacpp
bash compile.sh &> /dev/null
cd ${DIR}
echo "    Done."

echo ""
echo "TSB written in Python"
echo "---------------------"
bash upf/python/clients/python/run.sh

echo ""
echo "TSB written in Java via GRPC"
echo "-------------------"
cd ${DIR}/upf/python/clients/java/JClient
./gradlew --console plain -q runClient
cd ${DIR}

echo ""
echo "TSB written in Java"
echo "-------------------"
cd ${DIR}/upf/python/clients/java/JavacppClient
bash compile_jupf.sh &> /dev/null
bash run.sh
cd ${DIR}

echo ""
echo "TSB written in C++"
echo "------------------"
echo " -> Compiling..."
cd ${DIR}/upf/python/clients/cpp
bash compile.sh &> /dev/null
echo "    Done."
bash run.sh
cd ${DIR}

pkill -P ${UPF_SERVER_PID}; kill ${UPF_SERVER_PID}
kill -9 ${JAVA_SERVER_PID}

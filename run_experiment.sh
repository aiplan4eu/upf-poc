#!/bin/bash

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

echo ""
echo "TSB written in Python"
echo "---------------------"
bash upf/python/clients/python/run.sh

echo ""
echo "TSB written in Java"
echo "-------------------"
cd ${DIR}/upf/python/clients/java/JClient
./gradlew --console plain -q runClient
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


kill -9 ${UPF_SERVER_PID}
kill -9 ${JAVA_SERVER_PID}

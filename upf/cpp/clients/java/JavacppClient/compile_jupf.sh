#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

g++ -O3 -shared -fPIC -Wall -Werror -std=c++17 -I${DIR}/../../../ ${DIR}/jupf.cxx ${DIR}/../../../upf.so -o ${DIR}/libjupf.so

java -jar javacpp.jar UPFConfig.java
java -jar javacpp.jar UPF.java
javac -cp ./:javacpp.jar Main.java

#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

PYTHON_VERSION=`python3 -c "import sys; print('%s.%s' %(sys.version_info.major,sys.version_info.minor))"`

export CPLUS_INCLUDE_PATH="/usr/include/python${PYTHON_VERSION}:${DIR}/../../../../../pybind11/include"

java -jar javacpp.jar UPFConfig.java
java -jar javacpp.jar UPF.java
javac -cp ./:javacpp.jar Main.java

#!/bin/sh

java -jar javacpp.jar -header -classpath ../JPlanner/build/classes/java/main/:. PlannerWrapper.java

g++ -O3 -fPIC -std=c++17 -I../../../upf/cpp/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/ main.cxx linux-x86_64/libjniPlannerWrapper.so -L../../../upf/cpp -lupf -o main
g++ -O3 -fPIC -shared -std=c++17 -I../../../upf/cpp/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/ jplanner_upf.cxx -L./linux-x86_64 -L../../../upf/cpp -ljniPlannerWrapper -lupf -o jplanner_upf.so

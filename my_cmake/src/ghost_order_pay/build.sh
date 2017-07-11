#!/bin/bash

protoc  ../proto/message.proto --cpp_out=./src -I=../proto
protoc  ../proto/connect_server.proto --cpp_out=./src -I=../proto
mv -f ./src/message.pb.cc ./src/message.pb.cpp
mv -f ./src/connect_server.pb.cc ./src/connect_server.pb.cpp

mkdir -p bin		

rm -fr CMakeCache.txt
rm -fr CMakeFiles


cmake .
make clean;make

rm -fr CMakeCache.txt
rm -fr CMakeFiles

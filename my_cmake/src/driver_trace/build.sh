#!/bin/bash

#protoc  ../proto/connect_server.proto --cpp_out=./src -I=../proto
#mv -f ./src/connect_server.pb.cc ./src/connect_server.pb.cpp

mkdir -p bin		

rm -fr CMakeCache.txt
rm -fr Makefile
rm -fr cmake_install.cmake
rm -fr CMakeFiles

cmake .
make clean;make

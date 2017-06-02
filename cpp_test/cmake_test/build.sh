#!/bin/bash

#cd ../efnfw
#sh build.sh
#cd -

#cd ../common
#sh build.sh
#cd -

#protoc  ../proto/connect_server.proto --cpp_out=./src -I=../proto
#mv -f ./src/connect_server.pb.cc ./src/connect_server.pb.cpp

rm -fr CMakeCache.txt
rm -fr CMakeFiles
rm -fr cmake_install.cmake
rm -rf Makefile

mkdir -p bin

cmake .
make clean;make
rm -fr CMakeCache.txt
rm -fr CMakeFiles
rm -fr cmake_install.cmake
rm -rf Makefile

cmake -S . -B build -D CMAKE_BUILD_TYPE:STRING=${1:=Debug} -D CMAKE_C_COMPILER:STRING=${2:=gcc} -D CMAKE_CXX_COMPILER:STRING=${3:=g++}
cmake --build build

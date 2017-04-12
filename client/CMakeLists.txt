# Minimum CMake required
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive -g -Wno-literal-suffix -Wno-deprecated-declarations") 
SET(SERVER_PATH ${PROJECT_SOURCE_DIR}/../server)
project(CLIENT CXX)

add_custom_command(OUTPUT Protocol.pb.cc Protocol.pb.h
  COMMAND protoc
  ARGS --cpp_out . ${SERVER_PATH}/Protocol.proto -I${SERVER_PATH}
  DEPENDS ${SERVER_PATH}/Protocol.proto
  VERBATIM )
SET_SOURCE_FILES_PROPERTIES(Protocol.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion -Wno-shadow")
ADD_LIBRARY(proto Protocol.pb.cc)

add_custom_command(OUTPUT Protocol.grpc.pb.cc Protocol.grpc.pb.h
  COMMAND protoc
  ARGS --grpc_out . ${SERVER_PATH}/Protocol.proto -I${SERVER_PATH} --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin
  DEPENDS ${SERVER_PATH}/Protocol.proto
  VERBATIM)
ADD_LIBRARY(grpc_proto Protocol.grpc.pb.cc)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} /usr/include /usr/local/include/grpc/)   

link_directories(/usr/lib/x86_64-linux-gnu/ /usr/local/lib/) 

AUX_SOURCE_DIRECTORY(. SRC_LIST)

link_libraries(grpc_proto proto protobuf grpc++ grpc grpc++_reflection pthread)

FIND_LIBRARY(BS NAMES boost_system)
IF(BS)
    message(STATUS "Find out BOOST_SYSTEM")
    LINK_LIBRARIES(boost_system)
ELSE()
    message(STATUS "Can't find out BOOST_SYSTEM")
ENDIF()

add_executable(client ${SRC_LIST})
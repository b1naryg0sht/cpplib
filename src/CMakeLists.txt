cmake_minimum_required(VERSION 2.8)

project(CPPBrick)

SET(CMAKE_CXX_COMPILER "g++") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -gstabs+ -Wall -std=gnu++0x")

message(STATUS "project_source_dir is ${PROJECT_SOURCE_DIR}")

include_directories(.)
include_directories(../3rdparty/cryptopp/include)
include_directories(../3rdparty/tinyxml/include)

aux_source_directory(. SRC)

add_library(cppbrick STATIC ${SRC})

link_libraries(pthread)
link_directories(../3rdparty/cryptopp/lib)
link_directories(../3rdparty/tinyxml/lib)
link_directories(${PROJECT_BINARY_DIR}/build)
link_libraries(cryptopp tinyxml)
link_libraries(cppbrick)


add_executable(testThread test/testThread.cpp)
add_executable(testBase64 test/testBase64.cpp)
add_executable(testCryptopp test/testCryptopp.cpp)
add_executable(testStringUtils test/testStringUtils.cpp)
add_executable(testTVector test/testTVector.cpp) 
add_executable(testTVector2 test/testTVector2.cpp)
add_executable(testTQueue test/testTQueue.cpp)
add_executable(testTMapString test/testTMapString.cpp) 
add_executable(testTMapInt test/testTMapInt.cpp) 
add_executable(testTSingle test/testTSingleton.cpp) 
add_executable(testUdpClient test/testUdpClient.cpp) 
add_executable(testXMLParser test/testXMLParser.cpp)
add_executable(testTcpReactor test/testTcpReactor.cpp)
add_executable(testUdpReactor test/testUdpReactor.cpp)
add_executable(testSelectTimer test/testSelectTimer.cpp)
add_executable(testThreadMeet test/testThreadMeet.cpp)
add_executable(testCBLog test/testCBLog.cpp)
add_executable(testSimpleTcpClient test/testSimpleTcpClient.cpp)
add_executable(testSimpleTcpServer test/testSimpleTcpServer.cpp)
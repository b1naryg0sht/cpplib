cmake_minimum_required(VERSION 2.8)

project(yogurt)

SET(CMAKE_CXX_COMPILER "g++") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=gnu++0x")

include_directories(.) 
aux_source_directory(. SRC) 
add_library(yogurt STATIC ${SRC})

link_libraries(pthread)
link_directories(${PROJECT_BINARY_DIR}/build) 
link_libraries(yogurt)

add_executable(testOSThread test/testOSThread.cpp) 
#add_executable(testStringUtils test/testStringUtils.cpp) 
#add_executable(testThreadUtils test/testThreadUtils.cpp) 
#add_executable(testTimeStamp test/testTimeStamp.cpp) 
#add_executable(testTMsgQueue test/testTMsgQueue.cpp) 


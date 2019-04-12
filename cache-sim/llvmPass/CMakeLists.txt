cmake_minimum_required(VERSION 3.0)

## TODO: Den folgenden Pfad musst du anpassen und auf $PROJECT/... verweisen lassen.

set(LLVM_DIR "/home/leo/llvm/share/llvm/cmake/")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Testing")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

message(STATUS "Include dirs: ${LLVM_INCLUDE_DIRS}")

add_subdirectory(InsertPass)

cmake_minimum_required(VERSION 3.8)
project(c_calls_python)

set(CMAKE_C_STANDARD 99)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
add_definitions(-DUSE_PYTHON_3)
set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/opt/python3/Frameworks/Python.framework/Versions/3.6/lib/python3.6/config-3.6m-darwin -lpython3.6m -ldl -framework CoreFoundation")

set(SOURCE_FILES main.c ccpy.c ccpy.h)

add_executable(c_calls_python ${SOURCE_FILES})
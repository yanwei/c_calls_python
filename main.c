#include <stdio.h>
#include "ccpy.h"

int main(int argc, char *argv[]) {
    // python file path
    char *py_file = "/Users/yanwei/CLionProjects/c_calls_python/test001.py";

    // init python env with the main program path and the python file path
    CallPythonInit(argv[0], "/Users/yanwei/CLionProjects/c_calls_python");

    // test call python file
    CallPythonFile(py_file);

    printf("========================================\n");

    // test call python function
    char *input = "{\"no\": 1, \"name\": \"Runoob\", \"url\": \"http://www.runoob.com\"}";
    char output[CCPY_MAX_BUFSIZE] = "";
    CallPythonFunc("test001", "test_func2", input, output);
    printf("output = %s\n", output);

    return 0;
}

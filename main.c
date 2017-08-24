#include <stdio.h>
#include "ccpy.h"

int main(int argc, char *argv[]) {
    char *py_file = "/Users/yanwei/CLionProjects/c_calls_python/test001.py";

    CallPythonInit(argv[0], "/Users/yanwei/CLionProjects/c_calls_python");

    CallPythonFile(py_file);

    char *input = "{\"no\": 1, \"name\": \"Runoob\", \"url\": \"http://www.runoob.com\"}";
    char output[CCPY_MAX_BUFSIZE] = "";
    CallPythonFunc("test001", "test_func2", input, output);
    printf("output = %s", output);

    return 0;
}

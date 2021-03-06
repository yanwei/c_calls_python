/*
 * Call python file or python function from C.
 * Support both python2.7 and python3.6
 * Define USE_PYTHON_3 if you want to use python3.6, otherwise use python2.7 by default.
 *
 * Created by yanwei on 18/08/2017.
 */

#ifndef C_CALLS_PYTHON_CCPY_H
#define C_CALLS_PYTHON_CCPY_H

// const definition for the return values of the following functions
#define CCPY_ERR_SUCCESS            0
#define CCPY_ERR_FAIL               1
#define CCPY_ERR_FILE_NOT_FOUND     2
#define CCPY_ERR_FUNC_NOT_FOUND     3

#define CCPY_MAX_BUFSIZE            10240

/*
 * Call this function before calling any other functions.
 *
 * main_program_path: pass argv[0] of the main program to this argument.
 * python_path: path where the python files is stored
 */
#ifdef USE_PYTHON_3
int CallPythonInit(const char *main_program_path, const char *python_path);
#else
int CallPythonInit(char *main_program_path, const char *python_path);
#endif

/*
 * Execute a python(.py) file without any input or output data.
 * This is the simplest way to run some python code from C.
 * Just make sure the .py file is correct in syntax and semantic.
 *
 * py_file_path: full path of the .py file to be executed.
 */
int CallPythonFile(const char *py_file_path);

/*
 * Call a python function defined in a python file.
 *
 * py_file_path: full path of the .py file
 * func_name: name of the function to be called
 * input: input arguments as json format. Could be NULL if no input is needed.
 * output: output as json format. Could be NULL if no output is needed.
 */
int CallPythonFunc(const char *py_file_path, const char *func_name, const char *input, char output[CCPY_MAX_BUFSIZE]);

#endif //C_CALLS_PYTHON_CCPY_H

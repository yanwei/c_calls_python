/*
 * Created by yanwei on 18/08/2017.
 */

#include <Python.h>
#include "ccpy.h"

#ifdef USE_PYTHON_3
int CallPythonInit(const char *main_program_path, const char *python_path) {
#else
int CallPythonInit(char *main_program_path, const char *python_path) {
#endif
    setenv("PYTHONPATH", python_path, 1);

#ifdef USE_PYTHON_3
    wchar_t *program = Py_DecodeLocale(main_program_path, NULL);
    if (program == NULL)
        return CCPY_ERR_FAIL;
    Py_SetProgramName(program);
    PyMem_RawFree(program);
#else
    Py_SetProgramName(main_program_path);
#endif
    
    return CCPY_ERR_SUCCESS;
}

int CallPythonFile(const char *py_file_path) {
    Py_Initialize();
    if (!Py_IsInitialized())
        return CCPY_ERR_FAIL;

    FILE *fp = fopen(py_file_path, "r");
    if (fp == NULL) {
        Py_Finalize();
        return CCPY_ERR_FILE_NOT_FOUND;
    }

    int iRet = PyRun_SimpleFile(fp, py_file_path);

    fclose(fp);

    Py_Finalize();

    if (iRet == 0)
        return CCPY_ERR_SUCCESS;
    else
        return CCPY_ERR_FAIL;
}

int CallPythonFunc(const char *py_file_path, const char *func_name, const char *input, char output[CCPY_MAX_BUFSIZE]) {
#ifdef USE_PYTHON_3
    PyObject *pName;
#endif
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    Py_Initialize();
    if (!Py_IsInitialized())
        return CCPY_ERR_FAIL;

#ifdef USE_PYTHON_3
    pName = PyUnicode_FromString(py_file_path);
    if (pName == NULL) {
        Py_Finalize();
        return CCPY_ERR_FAIL;
    }
    pModule = PyImport_Import(pName);
    Py_XDECREF(pName);
#else
    pModule = PyImport_ImportModule(py_file_path);
#endif

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func_name);

        if (pFunc && PyCallable_Check(pFunc)) {
            if (input) {
                pArgs = PyTuple_New(1);
#ifdef USE_PYTHON_3
                pValue = PyUnicode_FromString(input);
#else
                pValue = PyString_FromString(input);
#endif
                PyTuple_SetItem(pArgs, 0, pValue);
            } else
                pArgs = NULL;
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_XDECREF(pArgs);
            if (pValue != NULL) {
                if (output) {
#ifdef USE_PYTHON_3
                    PyObject *pyStr = PyUnicode_AsASCIIString(pValue);
                    const char *c_str = PyBytes_AS_STRING(pyStr);
                    strncpy(output, c_str, CCPY_MAX_BUFSIZE - 1);
                    Py_XDECREF(pyStr);
#else
                    const char *c_str = PyString_AsString(pValue);
                    strncpy(output, c_str, CCPY_MAX_BUFSIZE - 1);
#endif
                }
                Py_XDECREF(pValue);
            } else {
                Py_XDECREF(pFunc);
                Py_XDECREF(pModule);
                PyErr_Print();

                Py_Finalize();
                return CCPY_ERR_FAIL;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            Py_Finalize();
            return CCPY_ERR_FUNC_NOT_FOUND;
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
        Py_Finalize();
        return CCPY_ERR_FILE_NOT_FOUND;
    }

    Py_Finalize();
    return CCPY_ERR_SUCCESS;
}

/*
 * Created by yanwei on 18/08/2017.
 */

#include <Python.h>
#include "ccpy.h"

int CallPythonInit(const char *main_program_path, const char *python_path) {
    setenv("PYTHONPATH", python_path, 1);

    wchar_t *program = Py_DecodeLocale(main_program_path, NULL);
    if (program == NULL)
        return CCPY_ERR_FAIL;

    Py_SetProgramName(program);

    PyMem_RawFree(program);
    return CCPY_ERR_SUCCESS;
}

int CallPythonFile(const char *py_file_path) {
    Py_Initialize();
    if (!Py_IsInitialized())
        return CCPY_ERR_FAIL;

    FILE *fp = fopen(py_file_path, "r");
    if (fp == NULL) {
        Py_FinalizeEx();
        return CCPY_ERR_FILE_NOT_FOUND;
    }

    int iRet = PyRun_SimpleFileEx(fp, py_file_path, 1);

    Py_FinalizeEx();

    if (iRet == 0)
        return CCPY_ERR_SUCCESS;
    else
        return CCPY_ERR_FAIL;
}

int CallPythonFunc(const char *py_file_path, const char *func_name, const char *input, char output[CCPY_MAX_BUFSIZE]) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    Py_Initialize();
    if (!Py_IsInitialized())
        return CCPY_ERR_FAIL;

    pName = PyUnicode_FromString(py_file_path);
    if (pName == NULL) {
        Py_FinalizeEx();
        return CCPY_ERR_FAIL;
    }

    pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func_name);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            if (input) {
                pArgs = PyTuple_New(1);
                pValue = PyUnicode_FromString(input);
                PyTuple_SetItem(pArgs, 0, pValue);
            } else
                pArgs = NULL;
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_XDECREF(pArgs);
            if (pValue != NULL) {
                if (output) {
                    PyObject *pyStr = PyUnicode_AsASCIIString(pValue);
                    const char *c_str = PyBytes_AS_STRING(pyStr);
                    strncpy(output, c_str, CCPY_MAX_BUFSIZE - 1);
                    Py_XDECREF(pyStr);
                }
                Py_XDECREF(pValue);
            } else {
                Py_XDECREF(pFunc);
                Py_XDECREF(pModule);
                PyErr_Print();

                Py_FinalizeEx();
                return CCPY_ERR_FAIL;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            Py_FinalizeEx();
            return CCPY_ERR_FUNC_NOT_FOUND;
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
        Py_FinalizeEx();
        return CCPY_ERR_FILE_NOT_FOUND;
    }

    Py_FinalizeEx();
    return CCPY_ERR_SUCCESS;
}

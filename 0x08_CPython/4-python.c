#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p) {
    if (!PyUnicode_Check(p)) {
        printf("[.] string object info\n");
        printf("  [ERROR] Invalid String Object\n");
        return;
    }


    const char *type = PyUnicode_IS_COMPACT_ASCII(p) ? "compact ascii" : "compact unicode object";

    
    Py_ssize_t length = PyUnicode_GET_LENGTH(p);

    
    const char *value = PyUnicode_AsUTF8(p);

    
    printf("[.] string object info\n");
    printf("  type: %s\n", type);
    printf("  length: %zd\n", length);
    printf("  value: %s\n", value);
}


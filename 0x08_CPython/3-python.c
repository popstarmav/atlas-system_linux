#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

/**
 * print_python_list - Prints basic information about Python lists
 * @p: A PyObject pointer (expected to be a Python list)
 */
void print_python_list(PyObject *p)
{
    if (!PyList_Check(p)) {
        printf("[ERROR] Invalid List Object\n");
        return;
    }

    Py_ssize_t size = PyList_Size(p);
    Py_ssize_t allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", size);
    printf("[*] Allocated = %ld\n", allocated);

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(p, i);
        const char *type_name = Py_TYPE(item)->tp_name;
        printf("Element %ld: %s\n", i, type_name);

        if (PyBytes_Check(item)) {
            print_python_bytes(item);
        } else if (PyFloat_Check(item)) {
            print_python_float(item);
        }
    }
}

/**
 * print_python_bytes - Prints basic information about Python bytes objects
 * @p: A PyObject pointer (expected to be a Python bytes object)
 */
void print_python_bytes(PyObject *p)
{
    if (!PyBytes_Check(p)) {
        printf("[ERROR] Invalid Bytes Object\n");
        return;
    }

    Py_ssize_t size = PyBytes_Size(p);
    char *string = PyBytes_AsString(p);

    printf("[.] bytes object info\n");
    printf("  size: %ld\n", size);
    printf("  trying string: %s\n", string ? string : "NULL");

    Py_ssize_t to_print = size < 10 ? size : 10;
    printf("  first %ld bytes: ", to_print);

    for (Py_ssize_t i = 0; i < to_print; i++) {
        printf("%02x", (unsigned char)string[i]);
        if (i < to_print - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

/**
 * print_python_float - Prints basic information about Python float objects
 * @p: A PyObject pointer (expected to be a Python float object)
 */
void print_python_float(PyObject *p)
{
    if (!PyFloat_Check(p)) {
        printf("[ERROR] Invalid Float Object\n");
        return;
    }

    double value = PyFloat_AsDouble(p);

    printf("[.] float object info\n");
    printf("  value: %.16g\n", value);
}


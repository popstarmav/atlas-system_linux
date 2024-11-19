#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);

void print_python_list(PyObject *p)
{
    Py_ssize_t size, allocated, i;
    const char *type;

    if (!PyList_Check(p))
    {
        fprintf(stderr, "[ERROR] Invalid List Object\n");
        return;
    }

    size = PyList_Size(p);
    allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    for (i = 0; i < size; i++)
    {
        PyObject *item = PyList_GetItem(p, i);
        type = item->ob_type->tp_name;
        printf("Element %zd: %s\n", i, type);

        if (PyBytes_Check(item))
            print_python_bytes(item);
    }
}

void print_python_bytes(PyObject *p)
{
    Py_ssize_t size, print_bytes;
    char *bytes_str;

    printf("[.] bytes object info\n");

    if (!PyBytes_Check(p))
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    size = PyBytes_Size(p);
    bytes_str = PyBytes_AsString(p);
    print_bytes = size < 10 ? size + 1 : 10;

    printf("  size: %zd\n", size);
    printf("  trying string: %s\n", bytes_str);
    printf("  first %zd bytes: ", print_bytes);

    for (Py_ssize_t i = 0; i < print_bytes; i++)
        printf("%02x%c", (unsigned char)bytes_str[i], i + 1 == print_bytes ? '\n' : ' ');
}


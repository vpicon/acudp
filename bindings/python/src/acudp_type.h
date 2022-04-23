#ifndef _ACUDP_PYTHON_EXTENSION_ACUDP_OBJ_H
#define _ACUDP_PYTHON_EXTENSION_ACUDP_OBJ_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    // Type fields
} ACUDPObject;

extern PyTypeObject ACUDPType;


#endif  // _ACUDP_PYTHON_EXTENSION_ACUDP_OBJ_H

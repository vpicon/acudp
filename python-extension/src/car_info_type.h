#ifndef _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H
#define _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    // Type fields
} CarInfoObject;

extern PyTypeObject CarInfoType;


#endif  // _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H

#ifndef _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H
#define _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    // Type fields
} HandshakeResponseObject;

extern PyTypeObject HandshakeResponseType;


#endif  // _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H

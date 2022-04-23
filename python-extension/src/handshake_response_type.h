#ifndef _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H
#define _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD
    PyObject *car_name;      // string
    PyObject *driver_name;   // string
    int identifier;
    int version;
    PyObject *track_name;    // string
    PyObject *track_config;  // string
} HandshakeResponseObject;

extern PyTypeObject HandshakeResponseType;

// C-level interface constructor for HandshakeResponse objects
HandshakeResponseObject *HandshakeResponse_constructor(
    PyObject *car_name,
    PyObject *driver_name,
    int identifier,
    int version,
    PyObject *track_name,
    PyObject *track_config);



#endif  // _ACUDP_PYTHON_EXTENSION_CAR_HANDSHAKE_RESPONSE_TYPE_H

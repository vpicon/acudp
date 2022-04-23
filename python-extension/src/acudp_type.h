#ifndef _ACUDP_PYTHON_EXTENSION_ACUDP_TYPE_H
#define _ACUDP_PYTHON_EXTENSION_ACUDP_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include "acudp.h"


typedef struct {
    PyObject_HEAD
    acudp_handle *acudp;                       // inner acudp-library handle
    acudp_client_subscription_t subscription;  // state of client current suscription 
} ACUDPObject;

extern PyTypeObject ACUDPType;


#endif  // _ACUDP_PYTHON_EXTENSION_ACUDP_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "acudp_obj.h"


static PyModuleDef acudpmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "acudp",
    .m_doc = "AC UDP client module.",
    .m_size = -1,
};


PyMODINIT_FUNC
PyInit_acudp(void)
{
    // Initialize Types
    if (PyType_Ready(&ACUDPType) < 0)
        return NULL;

    // Create module
    PyObject *m;
    m = PyModule_Create(&acudpmodule);
    if (m == NULL)  // Failed to create module
        return NULL;

    // Add types and functions to module
    Py_INCREF(&ACUDPType);
    if (PyModule_AddObject(m, "ACUDP", (PyObject *) &ACUDPType) < 0) {
        Py_DECREF(&ACUDPType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

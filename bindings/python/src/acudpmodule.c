#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


static PyModuleDef acudpmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "acudp",
    .m_doc = "AC UDP client module.",
    .m_size = -1,
};


PyMODINIT_FUNC
PyInit_acudp(void)
{
    PyObject *m = PyModule_Create(&acudpmodule);
    if (m == NULL)  // Failed to create module
        return NULL;

    // Add Objects and functions
    // ...

    return m;
}

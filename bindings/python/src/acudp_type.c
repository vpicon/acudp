#include "acudp_type.h"
#include "handshake_response_type.h"
#include "util.h"


static void
ACUDP_dealloc(ACUDPObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
ACUDP_new(PyTypeObject *type,
        PyObject *UNUSED(args), PyObject *UNUSED(kwds))
{
    ACUDPObject *self;
    self = (ACUDPObject *) type->tp_alloc(type, 0);

    // Initialize acudp library
    if (self != NULL) {
        int rc = acudp_init(&self->acudp);
        if (rc != ACUDP_OK) {
            acudp_exit(self->acudp);
            Py_DECREF(self);
            /* TODO: throw exception; */
            return NULL;
        }
    }

    return (PyObject *) self;
}


static PyObject *
ACUDP_send_handshake(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    // TODO: is a stub
    (void) self;
    return (PyObject *) HandshakeResponse_constructor(
                Py_BuildValue("s", "Audi"),
                Py_BuildValue("s", "vpicon98"),
                1,
                4,
                Py_BuildValue("s", "magione"),
                Py_BuildValue("s", "configs"));
}


static PyMethodDef ACUDP_methods[] = {
    {"send_handshake", (PyCFunction) ACUDP_send_handshake, METH_NOARGS,
     "Sends handshake to AC Server and returns the response in a HandshakeResponse object"
    },
    {NULL}  /* Sentinel */
};


PyTypeObject ACUDPType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "acudp.ACUDP",
    .tp_doc = PyDoc_STR("ACUDP Client Object"),
    .tp_basicsize = sizeof(ACUDPObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = ACUDP_new,
    /* .tp_init = (initproc) HandshakeResponse_init, */
    .tp_dealloc = (destructor) ACUDP_dealloc,
    /* .tp_members = HandshakeResponse_members, */
    .tp_methods = ACUDP_methods
};

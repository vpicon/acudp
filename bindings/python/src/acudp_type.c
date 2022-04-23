#include "acudp_type.h"

static void
ACUDP_dealloc(ACUDPObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
ACUDP_send_handshake(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    // TODO: is a stub
    (void) self;
    return Py_BuildValue("s", "stub");
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
    .tp_new = PyType_GenericNew,
    /* .tp_new = HandshakeResponse_new, */
    /* .tp_init = (initproc) HandshakeResponse_init, */
    .tp_dealloc = (destructor) ACUDP_dealloc,
    /* .tp_members = HandshakeResponse_members, */
    .tp_methods = ACUDP_methods
};

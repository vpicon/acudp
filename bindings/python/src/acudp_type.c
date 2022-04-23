#include "acudp_type.h"

static void
ACUDP_dealloc(ACUDPObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}

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
    /* .tp_members = HandshakeResponse_members */
};

/*
PyTypeObject HandshakeResponseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "acudp.HandshakeResponse",
    .tp_doc = PyDoc_STR("Reponse object to Handshake operation"),
    .tp_basicsize = sizeof(HandshakeResponseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = HandshakeResponse_new,
    .tp_init = (initproc) HandshakeResponse_init,
    .tp_dealloc = (destructor) HandshakeResponse_dealloc,
    .tp_members = HandshakeResponse_members
};
*/

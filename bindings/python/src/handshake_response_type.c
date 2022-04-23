#include "acudp_type.h"

PyTypeObject HandshakeResponseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "acudp.HandshakeResponse",
    .tp_doc = PyDoc_STR("Reponse object to Handshake operation"),
    .tp_basicsize = sizeof(HandshakeResponseObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

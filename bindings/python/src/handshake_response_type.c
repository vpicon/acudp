#include "handshake_response_type.h"
#include "util.h"


static void
HandshakeResponse_dealloc(HandshakeResponseObject *self)
{
    Py_XDECREF(self->car_name);
    Py_XDECREF(self->driver_name);
    Py_XDECREF(self->track_name);
    Py_XDECREF(self->track_config);
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
HandshakeResponse_new(PyTypeObject *type,
        PyObject *UNUSED(args), PyObject *UNUSED(kwds))
{
    HandshakeResponseObject *self;
    self = (HandshakeResponseObject *) type->tp_alloc(type, 0);

    // Initialize strings to "" and integers to 0
    if (self != NULL) {
        self->car_name = PyUnicode_FromString("");
        if (self->car_name == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        self->driver_name = PyUnicode_FromString("");
        if (self->driver_name == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        self->identifier = 0;
        self->version = 0;
        self->track_name = PyUnicode_FromString("");
        if (self->track_name == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        self->track_config = PyUnicode_FromString("");
        if (self->track_config == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *) self;
}


static int
HandshakeResponse_init(HandshakeResponseObject *self,
        PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"car_name", "driver_name",
        "identifier", "version", "track_name", "track_config", NULL};
    PyObject *car_name = NULL, *driver_name = NULL,
             *track_name = NULL, *track_config = NULL, *tmp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOiiOO", kwlist,
                    &car_name, &driver_name,
                    &self->identifier, &self->version,
                    &track_name, &track_config))
        return -1;

    if (car_name) {
        tmp = self->car_name;
        Py_INCREF(car_name);
        self->car_name = car_name;
        Py_XDECREF(tmp);
    }
    if (driver_name) {
        tmp = self->driver_name;
        Py_INCREF(driver_name);
        self->driver_name = driver_name;
        Py_XDECREF(tmp);
    }
    if (track_config) {
        tmp = self->track_config;
        Py_INCREF(track_config);
        self->track_config = track_config;
        Py_XDECREF(tmp);
    }
    if (track_name) {
        tmp = self->track_name;
        Py_INCREF(track_name);
        self->track_name = track_name;
        Py_XDECREF(tmp);
    }
    return 0;
}


static PyMemberDef HandshakeResponse_members[] = {
    {"car_name", T_OBJECT_EX, offsetof(HandshakeResponseObject, car_name),
        READONLY, "car name"},
    {"driver_name", T_OBJECT_EX, offsetof(HandshakeResponseObject, driver_name),
        READONLY, "driver name"},
    {"identifier", T_INT, offsetof(HandshakeResponseObject, identifier),
        READONLY, "identifier"},
    {"version", T_INT, offsetof(HandshakeResponseObject, version),
        READONLY, "version"},
    {"track_name", T_OBJECT_EX, offsetof(HandshakeResponseObject, track_name),
        READONLY, "track name"},
    {"track_config", T_OBJECT_EX, offsetof(HandshakeResponseObject, track_config),
        READONLY, "track config"},
    {NULL}  /* Sentinel */
};


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

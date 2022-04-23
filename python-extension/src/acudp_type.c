#include "acudp_type.h"
#include "handshake_response_type.h"
#include "util.h"


static void
ACUDP_dealloc(ACUDPObject *self)
{
    acudp_exit(self->acudp);
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
ACUDP_new(PyTypeObject *type, PyObject *UNUSED(args), PyObject *UNUSED(kwds))
{
    ACUDPObject *self;
    self = (ACUDPObject *) type->tp_alloc(type, 0);

    if (self != NULL) {
        // Initialize acudp library
        int rc = acudp_init(&self->acudp);
        if (rc != ACUDP_OK) {
            acudp_exit(self->acudp);
            Py_DECREF(self);
            /* TODO: throw exception; */
            return NULL;
        }

        // Init subscription state
        self->subscription = ACUDP_SUBSCRIPTION_NONE;
    }

    return (PyObject *) self;
}


static PyObject *
ACUDP_send_handshake(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    acudp_setup_response_t response;
    int rc = acudp_send_handshake(self->acudp, &response);
    if (rc != ACUDP_OK) {
        /* TODO: throw exception; */
        return NULL;
    }

    return (PyObject *) HandshakeResponse_constructor(
                Py_BuildValue("s", response.car_name),
                Py_BuildValue("s", response.driver_name),
                response.identifier,
                response.version,
                Py_BuildValue("s", response.track_name),
                Py_BuildValue("s", response.track_config));
}


static PyObject *
_client_subscribe(ACUDPObject *self, acudp_client_subscription_t subscription)
{
    if (self->subscription != ACUDP_SUBSCRIPTION_NONE) {
        /* TODO: throw exception; */
        return NULL;
    }

    int rc = acudp_client_subscribe(self->acudp, subscription);
    if (rc != ACUDP_OK) {
        /* TODO: throw exception; */
        return NULL;
    }

    self->subscription = subscription;

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
ACUDP_subsribe_to_updates(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    return _client_subscribe(self, ACUDP_SUBSCRIPTION_UPDATE);
}


static PyObject *
ACUDP_subsribe_to_spot(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    return _client_subscribe(self, ACUDP_SUBSCRIPTION_SPOT);
}


static PyObject *
ACUDP_send_dismiss(ACUDPObject *self, PyObject *Py_UNUSED(ignored))
{
    int rc = acudp_send_dismiss(self->acudp);
    if (rc != ACUDP_OK) {
        /* TODO: throw exception; */
        return NULL;
    }

    self->subscription = ACUDP_SUBSCRIPTION_NONE;

    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef ACUDP_methods[] = {
    {"send_handshake", (PyCFunction) ACUDP_send_handshake, METH_NOARGS,
     "Sends handshake to AC Server and returns the response in a HandshakeResponse object"
    },
    {"subscribe_to_updates", (PyCFunction) ACUDP_subsribe_to_updates, METH_NOARGS,
     "Subscribes this client to AC Server UPDATE events"
    },
    {"subscribe_to_spot", (PyCFunction) ACUDP_subsribe_to_spot, METH_NOARGS,
     "Subscribes this client to AC Server SPOT events"
    },
    {"send_dismiss", (PyCFunction) ACUDP_send_dismiss, METH_NOARGS,
     "Sends dismiss event to AC Server to unsubscribe the client"
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
    /* .tp_init = (initproc) ACUDP_init, */
    .tp_dealloc = (destructor) ACUDP_dealloc,
    .tp_methods = ACUDP_methods
};

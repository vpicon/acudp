#include "util.h"
#include "car_info_type.h"


static void
CarInfo_dealloc(CarInfoObject *self)
{
    Py_XDECREF(self->car_coordinates);
    Py_XDECREF(self->is_in_pit);
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
CarInfo_new(PyTypeObject *type,
        PyObject *UNUSED(args), PyObject *UNUSED(kwds))
{
    CarInfoObject *self;
    self = (CarInfoObject *) type->tp_alloc(type, 0);

    // Initialize bools to false, empty tuples and numerics to 0
    if (self != NULL) {
        self->speed_kmh = 0;
        self->car_position_normalized = 0;
        self->car_coordinates =  Py_BuildValue("(fff)", 0, 0, 0);  // tuple
        self->is_in_pit = PyBool_FromLong(0);  // false

        self->gas = 0;
        self->brake = 0;
        self->clutch = 0;
        self->engine_rpm = 0;
        self->steer = 0;
        self->gear = 0;
        self->cg_height = 0;

        self->lap_time_millis = 0;
        self->last_lap_millis = 0;
        self->best_lap_millis = 0;
        self->lap_count = 0;
    }

    return (PyObject *) self;
}


static PyMemberDef CarInfo_members[] = {
    {"speed_kmh", T_FLOAT, offsetof(CarInfoObject, speed_kmh),
        READONLY, "car speed in km/h"},
    /* {"driver_name", T_OBJECT_EX, offsetof(HandshakeResponseObject, driver_name), */
        /* READONLY, "driver name"}, */
    /* {"identifier", T_INT, offsetof(HandshakeResponseObject, identifier), */
        /* READONLY, "identifier"}, */
    /* {"version", T_INT, offsetof(HandshakeResponseObject, version), */
        /* READONLY, "version"}, */
    /* {"track_name", T_OBJECT_EX, offsetof(HandshakeResponseObject, track_name), */
        /* READONLY, "track name"}, */
    /* {"track_config", T_OBJECT_EX, offsetof(HandshakeResponseObject, track_config), */
        /* READONLY, "track config"}, */
    {NULL}  /* Sentinel */
};


PyTypeObject CarInfoType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "acudp.CarInfo",
    .tp_doc = PyDoc_STR("CarInfo telemetry data sent as UPDATE events from AC Server"),
    .tp_basicsize = sizeof(CarInfoObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = CarInfo_new,
    /* .tp_init = (initproc) CarInfo_init, */
    .tp_dealloc = (destructor) CarInfo_dealloc,
    .tp_members = CarInfo_members
};

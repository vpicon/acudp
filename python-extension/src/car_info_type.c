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
        self->speed_kmh = 0.0;
        self->car_position_normalized = 0.0;
        self->car_coordinates =  Py_BuildValue("(fff)", 0.0, 0.0, 0.0);  // tuple
        self->is_in_pit = PyBool_FromLong(0);  // false

        self->gas = 0.0;
        self->brake = 0.0;
        self->clutch = 0.0;
        self->engine_rpm = 0.0;
        self->steer = 0.0;
        self->gear = 0;
        self->cg_height = 0.0;

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
    {"car_position_normalized", T_FLOAT, offsetof(CarInfoObject, car_position_normalized),
        READONLY, "car position in circuit normalized (percentage of circuit done)"},
    {"car_coordinates", T_OBJECT_EX, offsetof(CarInfoObject, car_coordinates),
        READONLY, "absolute reference car coordinates (x, y, z)"},
    {"is_in_pit", T_BOOL, offsetof(CarInfoObject, is_in_pit),
        READONLY, "is car in pit stop"},

    {"gas", T_FLOAT, offsetof(CarInfoObject, gas),
        READONLY, "gas"},
    {"brake", T_FLOAT, offsetof(CarInfoObject, brake),
        READONLY, "brake"},
    {"clutch", T_FLOAT, offsetof(CarInfoObject, clutch),
        READONLY, "clutch"},
    {"engine_rpm", T_FLOAT, offsetof(CarInfoObject, engine_rpm),
        READONLY, "engine rpm"},
    {"steer", T_FLOAT, offsetof(CarInfoObject, steer),
        READONLY, "steer"},
    {"gear", T_INT, offsetof(CarInfoObject, gear),
        READONLY, "gear"},
    {"cg_height", T_FLOAT, offsetof(CarInfoObject, cg_height),
        READONLY, "cg height"},

    {"lap_time_millis", T_INT, offsetof(CarInfoObject, lap_time_millis),
        READONLY, "current lap time in milliseconds"},
    {"last_lap_millis", T_INT, offsetof(CarInfoObject, last_lap_millis),
        READONLY, "last lap time in milliseconds"},
    {"best_lap_millis", T_INT, offsetof(CarInfoObject, best_lap_millis),
        READONLY, "best lap time in milliseconds"},
    {"lap_count", T_INT, offsetof(CarInfoObject, lap_count),
        READONLY, "lap_count"},

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

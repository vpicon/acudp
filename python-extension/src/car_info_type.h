#ifndef _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H
#define _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>


typedef struct {
    PyObject_HEAD

    float speed_kmh;
    float car_position_normalized;
    PyObject *car_coordinates;  // Tuple(float, float, float)
    PyObject *is_in_pit;        // Bool

    float gas;
    float brake;
    float clutch;
    float engine_rpm;
    float steer;
    int   gear;
    float cg_height;

    int lap_time_millis;
    int last_lap_millis;
    int best_lap_millis;
    int lap_count;
} CarInfoObject;

extern PyTypeObject CarInfoType;


#endif  // _ACUDP_PYTHON_EXTENSION_CAR_INFO_TYPE_H

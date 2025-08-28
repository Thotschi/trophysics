# boundscheck=False, wraparound=False, cdivision=True, nonecheck=False
# write "cython:" in front of line above for these options to take effect
# but we use compiler directives in setup.py instead
import numpy as np
cimport numpy as np


cpdef np.ndarray[np.float64_t, ndim=1] c_rk2_step(
    double t,
    double dt,
    np.ndarray[np.float64_t, ndim=1] py_y,
    object py_ODE_func,
    object params):

    if py_y.ndim != 1:
        raise ValueError("Input py_y must be a 1D array")

    if py_y.dtype != np.float64:
        raise TypeError("Input py_y must be of dtype float64")

    if not py_y.flags['C_CONTIGUOUS']:
        py_y = np.ascontiguousarray(py_y, dtype=np.float64)

    if not callable(py_ODE_func):
        raise TypeError("py_ODE_func must be a callable")

    cdef int n = py_y.shape[0]
    cdef int i

    # Allocate arrays (C-contiguous by default)
    cdef np.ndarray[np.float64_t, ndim=1] sup = np.empty(n, dtype=np.float64)

    # Pointers for speed (analog to malloc. Therfore, arrays must be C-contiguous)
    cdef double* y_ptr = <double*> py_y.data
    cdef double* sup_ptr = <double*> sup.data

    sup[:] = py_ODE_func(t, py_y, params)
    for i in range(n):
        sup_ptr[i] = y_ptr[i] + 0.5 * dt * sup_ptr[i]

    sup[:] = py_ODE_func(t + 0.5 * dt, sup, params)
    for i in range(n):
        sup_ptr[i] = y_ptr[i] + 0.5 * dt * sup_ptr[i]

    return sup



cpdef np.ndarray[np.float64_t, ndim=1] c_rk4_step(
    double t,
    double dt,
    np.ndarray[np.float64_t, ndim=1] py_y,
    object py_ODE_func,
    object params):

    if py_y.ndim != 1:
        raise ValueError("Input py_y must be a 1D array")

    if py_y.dtype != np.float64:
        raise TypeError("Input py_y must be of dtype float64")

    if not py_y.flags['C_CONTIGUOUS']:
        py_y = np.ascontiguousarray(py_y, dtype=np.float64)

    if not callable(py_ODE_func):
        raise TypeError("py_ODE_func must be a callable")

    cdef int n = py_y.shape[0]
    cdef int i

    # Allocate arrays (C-contiguous by default)
    cdef np.ndarray[np.float64_t, ndim=1] k1 = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] k2 = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] k3 = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] k4 = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] sup = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] y_new = np.empty(n, dtype=np.float64)

    # Pointers for speed (analog to malloc. Therfore, arrays must be C-contiguous)
    cdef double* y_ptr = <double*> py_y.data
    cdef double* sup_ptr = <double*> sup.data
    cdef double* y_new_ptr = <double*> y_new.data
    cdef double* k1_ptr = <double*> k1.data
    cdef double* k2_ptr = <double*> k2.data
    cdef double* k3_ptr = <double*> k3.data
    cdef double* k4_ptr = <double*> k4.data

    k1[:] = py_ODE_func(t, py_y, params)
    for i in range(n):
        sup_ptr[i] = y_ptr[i] + 0.5 * dt * k1_ptr[i]

    k2[:] = py_ODE_func(t + 0.5 * dt, sup, params)
    for i in range(n):
        sup_ptr[i] = y_ptr[i] + 0.5 * dt * k2_ptr[i]

    k3[:] = py_ODE_func(t + 0.5 * dt, sup, params)
    for i in range(n):
        sup_ptr[i] = y_ptr[i] + dt * k3_ptr[i]

    k4[:] = py_ODE_func(t + dt, sup, params)
    for i in range(n):
        y_new_ptr[i] = y_ptr[i] + (dt / 6.0) * (
            k1_ptr[i] + 2.0 * k2_ptr[i] + 2.0 * k3_ptr[i] + k4_ptr[i]
        )

    return y_new
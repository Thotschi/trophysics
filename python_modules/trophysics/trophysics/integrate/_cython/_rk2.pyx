import numpy as np
cimport numpy as np

#-------------------------------------------------------------------------------------
# rk2

cpdef np.ndarray[np.float64_t, ndim=1] rk2_step(
    double t,
    double dt,
    np.ndarray[np.float64_t, ndim=1] py_y,
    object py_ODE_func,
    object params
):
    if not py_y.flags['C_CONTIGUOUS']:
        py_y = np.ascontiguousarray(py_y, dtype=np.float64)

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
# boundscheck=False, wraparound=False, cdivision=True, nonecheck=False
# write "cython:" in front of line above for these options to take effect
# but we use compiler directives in setup.py instead
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

#-------------------------------------------------------------------------------------
# rk4

cpdef np.ndarray[np.float64_t, ndim=1] rk4_step(
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

#-------------------------------------------------------------------------------------
# rk45

cdef class PyRHSWrapper:
    cdef object f
    cdef object params

    def __init__(self, f, params):
        self.f = f
        self.params = params

    cpdef eval(self, double t, double[:] y, double[:] dydt):
        cdef np.ndarray[np.float64_t, ndim=1] out = self.f(t, np.asarray(y), self.params)
        cdef Py_ssize_t i, n = out.shape[0]
        for i in range(n):
            dydt[i] = out[i]


cdef extern from "math.h":
    double fabs(double x)


# butcher tableau for RK45
cdef extern from *:
    """
    static const double A[6] = {
        0.0, 0.25, 0.375, 12.0/13.0, 1.0, 0.5
    };
    """
    const double A[6]


cdef extern from *:
    """
    static const double B[6][5] = {
        {0, 0, 0, 0, 0},
        {0.25, 0, 0, 0, 0},
        {3.0/32.0, 9.0/32.0, 0, 0, 0},
        {1932.0/2197.0, -7200.0/2197.0, 7296.0/2197.0, 0, 0},
        {439.0/216.0, -8.0, 3680.0/513.0, -845.0/4104.0, 0},
        {-8.0/27.0, 2.0, -3544.0/2565.0, 1859.0/4104.0, -11.0/40.0}
    };

    static const double C4[6] = {
        25.0/216.0, 0.0, 1408.0/2565.0, 2197.0/4104.0, -1.0/5.0, 0.0
    };

    static const double C5[6] = {
        16.0/135.0, 0.0, 6656.0/12825.0, 28561.0/56430.0, -9.0/50.0, 2.0/55.0
    };
    """
    const double B[6][5]
    const double C4[6]
    const double C5[6]


cpdef tuple rk45_step(
    double t,
    double dt,
    np.ndarray[np.float64_t, ndim=1] py_y,
    object py_ODE_func,
    object params,
    double tol
):
    if not py_y.flags['C_CONTIGUOUS']:
        py_y = np.ascontiguousarray(py_y, dtype=np.float64)

    cdef PyRHSWrapper rhs = PyRHSWrapper(py_ODE_func, params)

    cdef int n = py_y.shape[0]
    cdef int i, j, m

    cdef np.ndarray[np.float64_t, ndim=2] k = np.empty((6, n), dtype=np.float64, order="C")
    cdef np.ndarray[np.float64_t, ndim=1] sup = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] y4 = np.empty(n, dtype=np.float64)
    cdef np.ndarray[np.float64_t, ndim=1] y5 = np.empty(n, dtype=np.float64)

    # -------- memoryviews (entscheidend!) --------
    cdef double[:] y_mv = py_y
    cdef double[:] sup_mv = sup
    cdef double[:, :] k_mv = k

    cdef double* y_ptr = <double*> py_y.data
    cdef double* sup_ptr = <double*> sup.data
    cdef double* k_ptr = <double*> k.data
    cdef double* y4_ptr = <double*> y4.data
    cdef double* y5_ptr = <double*> y5.data

    # stages
    for i in range(6):
        for j in range(n):
            sup_ptr[j] = y_ptr[j]
            for m in range(i):
                sup_ptr[j] += dt * B[i][m] * k_ptr[m*n + j]

        # IN-PLACE, kein Assignment
        rhs.eval(t + A[i]*dt, sup_mv, k_mv[i])

    # combine solutions
    for j in range(n):
        y4_ptr[j] = y_ptr[j]
        y5_ptr[j] = y_ptr[j]
        for i in range(6):
            y4_ptr[j] += dt * C4[i] * k_ptr[i*n + j]
            y5_ptr[j] += dt * C5[i] * k_ptr[i*n + j]

    # infinity norm
    cdef double err = 0.0
    cdef double diff
    for j in range(n):
        diff = fabs(y5[j] - y4[j])
        if diff > err:
            err = diff

    cdef double dt_next
    if err == 0.0:
        dt_next = 2.0 * dt
    else:
        dt_next = dt * 0.9 * (tol / err)**0.25

    return (y5 if err < tol else py_y, dt_next)
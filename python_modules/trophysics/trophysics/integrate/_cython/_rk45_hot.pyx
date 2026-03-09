import numpy as np
cimport numpy as np

#-------------------------------------------------------------------------------------
# rk45

cdef extern from "math.h":
    double fabs(double x)


# butcher tableau for RK45

cdef extern from *:
    """
    static const double A[6] = {
        0.0, 0.25, 0.375, 12.0/13.0, 1.0, 0.5
    };

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
    const double A[6]
    const double B[6][5]
    const double C4[6]
    const double C5[6]


cdef tuple rk45_step(
    PyRHSWrapper rhs,
    double[:] y,
    double t,
    double[::1, ::1] k,
    double[:] sup,
    double[:] y4,
    double[:] y5,
    double dt,
    double tol,
    int n,
):

    cdef int i, j, m

    cdef double[:] sup_mv = sup
    cdef double[:, :] k_mv = k

    cdef double* y_ptr  = &py_y[0]
    cdef double* sup_ptr = &sup[0]
    cdef double* k_ptr  = &k[0, 0]
    cdef double* y4_ptr = &y4[0]
    cdef double* y5_ptr = &y5[0]

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
        diff = fabs(y5_ptr[j] - y4_ptr[j])
        if diff > err:
            err = diff

    cdef double dt_next
    if err == 0.0:
        dt_next = 2.0 * dt
    else:
        dt_next = dt * 0.9 * (tol / err)**0.25

    return (y5 if err < tol else py_y, dt_next)
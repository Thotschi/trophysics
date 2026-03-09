cdef class PyRHSWrapper:
    cdef object f
    cdef object params

    cpdef void eval(self, double t, double[:] y, double[:] dydt)
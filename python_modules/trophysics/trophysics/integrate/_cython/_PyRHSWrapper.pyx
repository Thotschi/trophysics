from ._PyRHSWrapper cimport PyRHSWrapper
import numpy as np
cimport numpy as np

cdef class PyRHSWrapper:

    def __init__(self, f, params):
        self.f = f
        self.params = params

    cpdef void eval(self, double t, double[:] y, double[:] dydt):
        cdef np.ndarray[np.float64_t, ndim=1] out = self.f(t, np.asarray(y), self.params)
        cdef Py_ssize_t i, n = out.shape[0]
        for i in range(n):
            dydt[i] = out[i]
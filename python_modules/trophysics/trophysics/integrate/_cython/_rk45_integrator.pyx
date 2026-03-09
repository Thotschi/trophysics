from ._PyRHSWrapper cimport PyRHSWrapper

cdef class RK45Integrator:
    def __cinit__(self, object py_ODE_func, double tol, object params=None):
        self.rhs = PyRHSWrapper(py_ODE_func, params)
        self.k = np.empty((6, n), dtype=np.float64)
        self.sup = np.empty(n, dtype=np.float64)
        self.y4 = np.empty(n, dtype=np.float64)
        self.y5 = np.empty(n, dtype=np.float64)


    cpdef ndarray rk45_integrate(
        self,
        double[:] y0,
        double dt0,
        double t0,
        double t_end,
        double[:] t_eval,
        double tol,
    ):
    
        cdef n = len(y0)

        cdef double t = t0
        y = y0
        dt = dt0

        while t < t_end:
            y_new, dt_suggested = rk45_step(self.rhs, y, t, self.k, self.sup, self.y4, self.y5, dt, tol, n)

            if y_new is not y:
                # Schritt akzeptiert
                t += dt
                y = y_new
                dt = dt_suggested
                store(t, y)
            else:
                # Schritt abgelehnt
                dt = dt_suggested
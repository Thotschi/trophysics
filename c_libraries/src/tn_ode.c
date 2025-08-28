#include "t_numerics_intern.h"

void
tn_euler_step (double t, double dt,
               double* y, ODE_FUNC ode_func,
               int dim, void *params)
{
    double* dy = (double *) malloc(dim * sizeof(double));
    exit_if_NULL (dy);
    
    // assignes the current state vektor y to tangential vector dy
    ode_func (t, y, dy, params);
    /*loop calculates state vector y as old state vector plus
     * small change in position and velocity in time step dt*/
    for (int i = 0; i < dim; ++i) {
        y[i] = dy[i] * dt + y[i]; 
    }

    free(dy);
}

void
tn_rk2_step(double t, double dt,
            double* y, ODE_FUNC ode_func,
            int dim, void *params)
{
    double* dy = (double *) malloc(dim * sizeof(double));
    exit_if_NULL (dy);

    double* sup = (double *) malloc(dim * sizeof(double));
    exit_if_NULL (sup);

    ode_func (t, y, dy, params);

    //calculate input for next ODE while dt * dy is k1
    for (int i = 0; i < dim; i++) {
        sup[i] = y[i] + 0.5 * (dt * dy[i]);
    }

    /* because ODE_FUNC does not alter y we can use dy as input anew
     * so that now the needed values are written into dy */
    ode_func (t + 0.5 * dt, sup, dy, params);

    for (int i = 0; i < dim; i++) {
        y[i] = y[i] + dt * dy[i];
    }

    free(dy);
    free(sup);
}

void
tn_rk4_step (double t, double dt,
             double* y, double* kArray,
             ODE_FUNC ode_func, int dim,
             void *params)
{
    /* allocate kArray before applying this function using:
     * double* kArray = (double *) malloc(5 * dim * sizeof(double)); */
    double* k1 = kArray + 0 * dim;
    double* k2 = kArray + 1 * dim;
    double* k3 = kArray + 2 * dim;
    double* k4 = kArray + 3 * dim;
    double* sup= kArray + 4 * dim;

    /* calculation k1 */
    ode_func (t, y, k1, params);
    // vektorial addition as input for next run of ODE
    for (int i = 0; i < dim; i++) {
        sup[i] = 0.5 * k1[i] * dt + y[i];
    }

    /* calculation k2 */
    ode_func (t + 0.5 * dt, sup, k2, params);
    for (int i = 0; i < dim; i++) {
        sup[i] = 0.5 * k2[i] * dt + y[i];
    }

    /* calculation k3 */
    ode_func (t + 0.5 * dt, sup, k3, params);
    for (int i = 0; i < dim; i++) {
        sup[i] = k3[i] * dt + y[i];
    }
    
    /* calculation k4 */
    ode_func (t + dt, sup, k4, params);

    // actual stepper that calculates next current state vector
    for (int i = 0; i < dim; i++) {
        y[i] += (dt/ 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    }
}

void
tn_vv_step (double t, double dt,
            double* y, ODE_FUNC ode_func,
            int dim, void *params)
{
    double* dy = (double *) malloc(dim * sizeof(double));
    exit_if_NULL(dy);

    double* dy_next = (double *) malloc(dim * sizeof(double));
    exit_if_NULL(dy_next);

    /* because we have one for-loop for position and one for second half
     * of y for velocity we need to abort calculation if dim % 2 != 0:
     * Therefore, we exit the complete programm */
    if (dim % 2 != 0) {
        tp_raiseError("Dimension of y must not be odd in Velocity-Verlet!\n");
    }

    ode_func(t, y, dy, params);
    
    for (int i = 0; i < dim / 2; ++i) {
        /* r(t) + v(t)*dt + 0.5*a(t)*dt^2 */
        y[i] = y[i] + y[dim / 2 + i] * dt + 0.5 * dy[dim / 2 + i] * dt * dt;
    }

    ode_func(t, y, dy_next, params);

    for (int i = dim / 2; i < dim; ++i) {
        /* v(t) + 0.5*(a(t + dt) + a(t))*dt */
        y[i] = y[i] + 0.5 * (dy_next[i] + dy[i]) * dt;
    }

    free(dy_next);
    free(dy);
}

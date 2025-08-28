import numpy as np

#rk2
def py_rk2_step (t: float, dt: float, y: np.array, ode_func, params):
  
      # call the ODE function
    dy = ode_func(t, y, params) # our readme specifies that the return value of the ODE function is a numpy array

    k1 = y + 0.5 * dt * dy #vectorized operation, no need for a loop

     # call the ODE function again
    dy = ode_func(t + 0.5 * dt, k1, params)
    return y + dt * dy

def py_rk4_step (t: float, dt: float, y: np.array, ode_func, params):
    '''
    Legible version
      # call the ODE function here for k1
    k1 = ode_func(t, y, params)
    support = y + 0.5 * dt * k1

    # call ODE for k2
    k2 = ode_func(t + dt / 2, support, params)
    
    support = y + 0.5 * dt * k2

    # call ODE for k3
    k3 = ode_func(t + dt / 2, support, params)
    
    support = y +  dt * k3
    
    # call ODE for k4
    k4 = ode_func(t + dt, support, params)

    #actual state update
    y += (dt / 6.0) * ( k1 + 2 * k2 +2 * k3 + k4)
    '''

    #optimized version
    k1 = ode_func(t, y, params)

    support = y  # reuse input array references (no new allocations)
    k2 = ode_func(t + 0.5 * dt, support + 0.5 * dt * k1, params)

    k3 = ode_func(t + 0.5 * dt, support + 0.5 * dt * k2, params)

    k4 = ode_func(t + dt, support + dt * k3, params)

    y += (dt / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4)
    return y

def py_rk45_step(t, dt, y, ode_func, params, sigmaokay):
    # butcher table
    a = np.array([0, 1 / 4, 3 / 8, 12 / 13, 1, 1 / 2])
    b = np.array([
        [0, 0, 0, 0, 0],
        [1 / 4, 0, 0, 0, 0],
        [3 / 32, 9 / 32, 0, 0, 0],
        [1932 / 2197, -7200 / 2197, 7296 / 2197, 0, 0],
        [439 / 216, -8, 3680 / 513, -845 / 4104, 0],
        [-8 / 27, 2, -3544 / 2565, 1859 / 4104, -11 / 40]
    ])
    c4 = np.array([25 / 216, 0, 1408 / 2565, 2197 / 4104, -1 / 5, 0])
    c5 = np.array([16 / 135, 0, 6656 / 12825, 28561 / 56430, -9 / 50, 2 / 55])

    # Compute all ks
    k = np.empty((6, len(y)))
    for i in range(6):
        yi = y + dt * np.dot(b[i, :i], k[:i])
        k[i] = ode_func(t + a[i] * dt, yi, params)

    # 4th and 5th order estimates
    y4 = y + dt * np.dot(c4, k)
    y5 = y + dt * np.dot(c5, k)

    # Error estimate and adaptive time step
    err = np.linalg.norm(y5 - y4, ord=np.inf)
    #safety = 0.9
    if err == 0:
        dt_next = dt * 2
    else:
        dt_next = dt * 0.9 * (sigmaokay / err) ** 0.25

    #if error okay, return 5th order calculation
    if err < sigmaokay:
        return y5, dt_next
    else:
        return py_rk45_step(t, dt_next, y, ode_func, params, sigmaokay)
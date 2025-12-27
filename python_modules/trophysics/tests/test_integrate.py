# imports
import numpy as np
import time
import matplotlib.pyplot as plt
import sys
import subprocess

from trophysics.integrate import rk45_step



#---------------------------------------------------------------------------------------------------------------------


# def activate_latex(*packages):
#     plt.rcParams['text.usetex'] = True
#     # import helpful usepackages
#     for package in packages:
#         plt.rcParams['text.latex.preamble'] += r'\usepackage{' + package + '}\n'
#     plt.rcParams['text.latex.preamble'] = r'\usepackage{xfrac}'

# activate_latex()

def set_std_fontsize (**kwargs):
    plt.rcParams['font.size'] = kwargs.get("font_size", 16)
    plt.rcParams['axes.labelsize'] = kwargs.get("axes_labelsize", 18)
    plt.rcParams['xtick.labelsize'] = kwargs.get("xtick_labelsize", 14)
    plt.rcParams['ytick.labelsize'] = kwargs.get("ytick_labelsize", 14)
    plt.rcParams['legend.fontsize'] = kwargs.get("legend_fontsize", 16)
    plt.rcParams['figure.titlesize'] = kwargs.get("figure_titlesize", 16)

set_std_fontsize()

#---------------------------------------------------------------------------------------------------------------------
# simluation parameters and ode

dim = 4 # 0
ph_g = 6.674e-11; # in m^3 kg^-1 s^-2 # 1
v_median_titan = 5.571e+03; # in m s^-1 # 2
m_titan = 1.345e+23; # in kg # 3
t_titan = 1.379e+6; # period duration in s # 4
r_median_titan = 1.222e+09; # in m # 5
m_saturn = 5.683e+26; # in kg  6
params = np.array([dim, ph_g, v_median_titan, m_titan, t_titan, r_median_titan, m_saturn], dtype=np.float64)
params = np.ascontiguousarray(params)


# ODE
def ODE_titan_2d (t, y, params) -> np.ndarray:
    # substitute for more intuitive names
    dim = params[0]
    ph_g = params[1]
    m_saturn = params[6]

    dy = np.empty(int(dim), dtype=np.float64) # C-contiguous

    # vector y has components {x, y, v_x, v_y}, dy = {v_x, v_y, a_x, a_y}
    dy[0] = y[2]
    dy[1] = y[3]
    # a_x = - G*M*x / r^3
    r = np.sqrt(y[0]**2 + y[1]**2)
    coefficient = (ph_g * m_saturn) / (r**3)
    dy[2] = - coefficient * y[0]
    dy[3] = - coefficient * y[1]
    return dy

#---------------------------------------------------------------------------------------------------------------------
# helper functions for simulation

# init function
def initial_conditions_titan_2d(dt=1e-5 * t_titan) -> tuple:
    t = 0.0
    y = np.array([r_median_titan, 0.0, 0.0, v_median_titan], dtype=np.float64)
    y = np.ascontiguousarray(y)
    return t, dt, y

def run_simulation(ode_func, stepper, dt, ode_impl,
                   steppername, simtime = 8 * t_titan,
                   print_time = True) -> tuple:
    t, dt, y = initial_conditions_titan_2d(dt)

    x_values = [y[0]]
    y_values = [y[1]]

    start_time = time.time()
    while t < simtime:
        y = stepper(t, dt, y, ode_func, params)
        t += dt
        x_values.append(y[0])
        y_values.append(y[1])
    end_time = time.time()
    exec_time = end_time - start_time
    if print_time:
        print("  "+steppername+" loop execution time with " +ode_impl+" ode: {:.3f} seconds".format (exec_time))
    return x_values, y_values, exec_time

#---------------------------------------------------------------------------------------------------------------------

def part_d():
    """
    make simulation with rk45 in python and plot trajectory
    """
    print("> Running Part D: simulation with RK45 in python and plotting trajectory...")

    runtime = 8 * t_titan
    # Timing the RK45 stepper in python
    t, dt, y = initial_conditions_titan_2d()

    sigma = 10

    x45_values = [y[0]]
    y45_values = [y[1]]

    start_time = time.time()
    while t < runtime:
        y, dt = rk45_step(t, dt, y, ODE_titan_2d, params, sigma)
        t += dt
        x45_values.append(y[0])
        y45_values.append(y[1])
    end_time = time.time()
    print("  RK45 loop execution time: {:.3f} seconds".format (end_time - start_time))

    # Plotting the results 
    plt.figure(figsize=(8, 11.5))
    plt.scatter(x45_values, y45_values, marker='o', s = 10, color='blue', label='Titan Trajectory')
    plt.scatter(0, 0, marker = ".", s = 20, color = "darkviolet", label = "Saturn")
    plt.scatter(0, 0, s = 60, facecolor = "None", edgecolor = "darkviolet")

    plt.xlabel('X (m)')
    plt.ylabel('Y (m)')

    plt.ylim(-1.5 * r_median_titan, 1.5 * r_median_titan)

    plt.title('2D Motion of Titan calculated by python with RK45')
    plt.legend()
    plt.grid()

    plt.tight_layout()

    plt.show()

#=====================================================================================================================
# define what to execute when calling this script

def exec_all():
    part_d()

# if .py file is called as main (e.g. not imported as module), following block is executed
if __name__ == "__main__":
    val_options = {
        "d": part_d,
        "all": exec_all
    }
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            if arg in val_options:
                val_options[arg]()
            else:
                print("> Unknown option: ", arg)
    else:
        exec_all()

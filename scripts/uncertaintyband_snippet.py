import matplotlib.pyplot as plt
import numpy as np
from jacobi import propagate
from numba_stats import norm

from iminuit import Minuit
from iminuit.cost import LeastSquares

#--------------------------------------------------------


def get_uncertainty_band(x, fit):
    y, ycov = propagate(lambda p: model(x, p), fit.values, fit.covariance)
    print(ycov)
    yerr_prop = np.diag(ycov) ** 0.5
    return y, y - yerr_prop, y + yerr_prop

#--------------------------------------------------------

# generate toy sample
rng = np.random.default_rng(1)

x = np.linspace(0, 10, 11)

y_values = np.exp(-0.2 * x) + np.random.normal(0, 0.05, size=x.size)
y_err = np.full_like(y_values, 0.05)






# define model and cost function
def model(x, par):
    return par[0] * np.exp(par[1] * x)


cost = LeastSquares(x, y_values, y_err, model)

# fit the model
m = Minuit(cost, (1, -0.3))
m.migrad()
m.hesse()



# plot everything
plt.errorbar(x, y_values, y_err, fmt="o", label="data", zorder=0)
y, y_lower, y_upper = get_uncertainty_band(x, m)

plt.plot(x, y, lw=3, label="fit")
plt.fill_between(x, y_lower, y_upper, facecolor="C1", alpha=0.5, label="uncertainty band", zorder=-1)

plt.legend(
    frameon=False,
    title=f"$n = {m.values[0]:.2f} +/- {m.errors[0]:.2f}$\n"
    f"$\\mu = {m.values[1]:.2f} +/- {m.errors[1]:.2f}$\n"
);

plt.savefig("uncertaintyband_snippet.png", dpi=300)


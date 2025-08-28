#include "t_numerics_intern.h"

//################################################################################
// stochastic

int
tn_binomialCoeff (const unsigned int n, const unsigned int k)
{
    if (k > n) {
        tp_raiseWarning ("In binomial coefficient k should not be greater than n. "
            "0 is returned.\n  But you might want to check your calculations.\n");
        return 0;
    }

    // base conditions
    if (k == 0 || k == n)
        return 1;

    if (k == 1 || k == n - 1)
        return n;

    // recursively add the value 
    return tn_binomialCoeff (n - 1, k - 1) + tn_binomialCoeff (n - 1, k);
}

// probability distributions

double
tn_binomial_distribution (int k, const unsigned int n, const double p)
{
    return tn_binomialCoeff (n, k) * pow(p, k) * pow(1 - p, n - k);
}

double
tn_normal_distribution (double x, void *params)
{
    double *array = (double *)params;
    double mu = array[0];
    double sigma = array[1];
    return (1.0 / (sigma * sqrt(2.0 * M_PI))) * exp(-0.5 * ((x - mu) / sigma)
            * ((x - mu) / sigma));
}

double
tn_cumulative_distr (double x, void *params)
{
    double *array = (double *)params;
    double mu = array[0];
    double sigma = array[1];
    return 0.5 * (1 + erf((x - mu) / (sigma * sqrt(2))));
}

t_array*
tn_rand_alloc (int n, double sigma, int seed)
{
    t_array* p = t_array_alloc(n);

    gsl_rng *r;
    const gsl_rng_type *T;
    gsl_rng_env_setup();

    T = gsl_rng_mt19937;
    r = gsl_rng_alloc (T);
    // seed the rng r 
    gsl_rng_set(r, seed);

    for (int i = 0; i < n; i++) {
        p->ptr[i] = gsl_ran_gaussian (r, sigma);
    }
    gsl_rng_free(r);
    return p;
}

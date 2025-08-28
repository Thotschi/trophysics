#include "t_numerics_intern.h"

//================================================================================
//    analysis
//================================================================================

//-----------------------------------
// selection of functions
//-----------------------------------

long int
tn_factorial (long int n)
{
    if (n == 0) {
        return 1;
    } else {
        return n * tn_factorial (n - 1);
    }
}

//--------------------------------------------------------------------------------
// algorithms for solving equations
//--------------------------------------------------------------------------------

Tuple
tn_solve_quadratic_real (double a, double b, double c)
{
    Tuple solution;

    if (b > 0) {
        solution.x1 = (2.0 * c) / (-b - sqrt(b * b + 4.0 * a * c));
        solution.x2 = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
    } else {
        solution.x1 = (-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
        solution.x2 = (2.0 * c) / (-b + sqrt(b * b + 4.0 * a * c));
    }
    
    return solution;
}

double
tn_find_root (STD_FUNC func, double x0, double dx,
              double tol, int max_iter, double close,
              void *params)
{
    /* filter 0 as root because when calculating relative error
     * we devide by current x which in this case would be 0 */
    if (func(0, params) == 0) {
        printf("> In Newton-Raphson 0 is a root.\n");
        printf("  Make sure your initial guess is far enough away from 0 "
            "to find a different root!\n");
        return 0;
    }

    double x = x0;
    double x_old;
    int iter = 0;
    while (iter <= max_iter) {
        x_old = x;
        x -= func(x, params) / tn_diff_1 (func, x, dx, params);
        if (fabs((x - x_old) / x) <= tol) {
            break;
        }
        iter++;
    }

    if (iter == max_iter) {
        tp_raiseWarning ("In Newton-Raphson root of function was found reaching "
            "given maximum iterations!\n");
    }
    if (isnan(x) == true) {
        tp_raiseWarning ("Result of Newton-Raphson is nan. Following values of "
            "this programm might be meaningless!\n");
    }
    if (fabs(func(x, params)) > close) {
        tp_raiseWarning ("Function at root found by Newton-Raphson has greater "
            "deviation than wanted!\n");
    }
    return x;
}

//--------------------------------------------------------------------------------
// numerical integration
//--------------------------------------------------------------------------------

double
tn_integrate_midpoint (STD_FUNC integrand, double a,
                       double b, double dx,
                       void *params)
{
    
    double sum = 0;
    // number of iterations rounded up because that way no precision is lost
    int N = ceil((b - a) / dx);
    // recalculate dt from rounded up N so that x ends on right integral limit
    dx = (b - a) / N;

    for (int k = 0; k < N; ++k) {
        double x = a + k * dx;
        double A = integrand(x, params) * dx;
        sum += A;
    }   

    return sum;
}


double
tn_integrate_simpson (STD_FUNC integrand, double a,
                      double b, double dx,
                      void *params)
{
    double sum = 0;

    int N = ceil((b - a) / dx);
    dx = (b - a) / N;

    double x;
    double summand;
    //Simpson consists of sum over S from 0 to N-1
    for (int k = 0; k <= N-1; ++k) {
        x = a + k * dx;
        summand = (dx/6.0) * (integrand (x, params)
            + 4*integrand (x + 0.5 * dx, params) 
            + integrand (x + dx, params));
        sum += summand;
    } 

    return sum;
}

double complex
tn_fourier_transform (double complex integrand(double, void *),
                      double m, double k, double dt,
                      void *params)
{
    double complex sum = 0;
    double complex summand;
    int N = ceil((2 * m) / dt);
    dt = (2 * m) / N;

    /* integration follows principle of Simpson */
    for (int i = 0; i < N; i++) {
        double t = -m + i * dt;
        summand = integrand (t, params) * cexp(-I*k*t)
                           + 4 * integrand (t + 0.5 * dt, params)
                           * cexp(-I*k*(t + 0.5 * dt))
                           + integrand (t + dt, params)
                           * cexp(-I*k*(t + dt));
        sum += summand;
    }
    return sum * (dt/6.0);
}

//--------------------------------------------------------------------------------
//    numerical differentiation
//--------------------------------------------------------------------------------

double
tn_diff_1 (STD_FUNC func, double x, double dx, void *params)
{
    return (func(x + dx, params) - func(x - dx, params)) / (2.0 * dx);
}

double
tn_diff_2 (STD_FUNC func, double x, double dx, void *params)
{
    return (func(x + dx, params) - 2.0 * func(x, params)
           + func(x - dx, params)) / (dx * dx);
}

/* t_numerics.h
 * 
 * -------------------------------------------------------------------------------
 * Purpose:
 * This header contains several algorithms for numerical calculations and 
 * standard operations that arise in physics. The algorithms fall into the 
 * following categories:
 * - linear algebra
 * - (functional) analysis
 * - differential equations
 * - stochastics
 * 
 * Usage:
 * Include this header in your project to access commonly used numeric utilities
 * without re-implementing basic algorithms.
 * 
 * Contributions and Feedback:
 * If you encounter bugs, have suggestions, or want to contribute, please reach out.
 * Contact: k.reinisch@stud.uni-goettingen.de
 * 
 * Date: 2025-08-14
 * Version: 0.7
 * 
 * License: CC BY-NC-SA 4.0
 * 
 * General notes: All parameters exept of the main argument of continuous
 * functions should be passed as void pointer of params. This is so that
 * all continuous functions correspond to the same function pointer which
 * is passed to algorithms that handle said functions
 * (e.g. simpson integrator).
 * 
 */

#ifndef T_NUMERICS_H
#define T_NUMERICS_H

/* combines math.h and complex.h headers */
#include <tgmath.h>
// random number generators
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <gsl/gsl_matrix.h>

#include "t_programcontrol.h"

//################################################################################
// arrays

// opaque pointer
typedef struct t_array t_array;

/*-- alloc t_array --*/
t_array* t_array_alloc (size_t len);

/*-- increment refcnt from t_array --*/
void t_array_ref(t_array* arr);

/*-- unref t_array --*/
void t_array_unref (t_array* arr);

/*--- read value from t_array ---*/
double t_array_get (t_array* arr, size_t index);

/*--- write value into t_array ---*/
void t_array_set (t_array* arr, size_t index, double value);

/* --- copy data into t_array from any source ---
 * stack or heap */
void t_array_copy_from_any (t_array* arr,
                            const double* data,
                            size_t len);

void t_array_assign(t_array** dest, t_array* src);

void t_array_copy_t_array(t_array* dest, const t_array* src);

#define T_ARRAY_FREE(arr) \
    do { \
        t_array_unref(arr); \
        arr = NULL; \
    } while (0)

/* distribute equidistant numbers between start and stop in array p of size n */
t_array* tn_linspace_alloc (double start, double stop, unsigned int n);

/* fill array p of len n with numbers with difference step */
t_array* tn_arange_alloc (double start, double step, unsigned int n);

/* fill array p of len n with numbers with logarithmic difference */
t_array* tn_logspace_alloc (double start, double stop, unsigned int n);

//--------------------------------------------------------------------------------
// matrices

// opaque pointer
typedef struct t_matrix t_matrix;

t_matrix* t_matrix_alloc(size_t rows, size_t cols);

t_matrix* t_matrix_create_from_gsl_matrix (const gsl_matrix* src);

void t_matrix_ref (t_matrix* m);

void t_matrix_unref (t_matrix* m);

void t_matrix_assign_t_array (t_matrix* m, t_array* heap_ptr);

double t_matrix_get (t_matrix* m, size_t i, size_t j);

/*----- modify matrix element at (i, j) -----*/
void t_matrix_set (t_matrix* m, size_t i, size_t j, double value);

/*----- use GSL functions:  -----
 * simply pass get_gsl_matrix(m) to GSL functions.
 * GSL view and data pointer are kept in sync. */
gsl_matrix* t_matrix_get_gsl_matrix(t_matrix* m);

/*-- copy matrix from stack --*/
void t_matrix_copy_from_array (t_matrix* m, const double* stack_ptr);

void t_matrix_copy_from_t_array (t_matrix* m, const t_array* arr);

void t_matrix_copy (t_matrix* dest, const t_matrix* src);

void t_matrix_copy_from_gsl_matrix (t_matrix* m, const gsl_matrix* src);

#define T_MATRIX_FREE(m) \
    do { \
        t_matrix_unref(m); \
        m = NULL; \
    } while (0)

//################################################################################
// LinAlg

// if one rather likes to handle vectors as tuples
typedef struct {
    double x;
    double y;
} Vec_2D;

typedef struct {
    double x;
    double y;
    double z;
} Vec_3D;

enum {
    X = 0,
    Y = 1,
    Z = 2
};

/*----standard dot product----*/
double tn_dot_product (const t_array* x, const t_array* y);

/*---quadratic matrix-vector product b = A * v ---*/
void tn_matrix_dot_vector (t_matrix* a, const t_array* v, t_array* b);

/*---quadratic nxn matrix-matrix product c = a * b ---*/
void tn_matrix_dot_matrix (t_matrix* a, t_matrix* b, t_matrix* c);

/*---distance between two points whom location vectors y1, y2 point to---*/
double tn_vec_dist (t_array* y1, t_array* y2);

/*---function calculates length of 3-dim vector---*/
/* uses norm induced by standard dot product */
double tn_len_vec_3d (t_array* y);

/*---function calculates length of 2-dim vektor---*/
double tn_len_vec_2d (t_array* y);

/*---calculates length of vector of any dimension---*/
double tn_len_vec (t_array* y);

/*-----make unit vector-----*/
void tn_norm_vec (t_array* v);

/*--norms vector so that sum of components == 1--*/
// mainly for probabilities
void tn_norm_vec_sum_1 (t_array* v);

/*---prints vector with name of vector---
 * format:
 *   vec_name[0]: 5.0
 *   vec_name[1]: 3.14
 *   ... */
void tn_print_vec (t_array* v, char vec_name[]);

/*---prints matrix with brackets, r (c) ist number of rows (colmums)---
 * format:
 *   ⌈  4   5   2   ⌉
 *   |  7   1   9   |
 *   ⌊  3   8   6   ⌋ */
void tn_print_matrix (t_matrix* m);

//--------------------------------------------------------------------------------
// algorithm to solve system of linear equations

// iteration step of Gauß-Seidel
void tn_gauss_seidel_step (const t_matrix* m,
                           const t_array* b, t_array* v);

/*--solves system of linear equations with solution vector v in form Av = b --*/
void tn_gauss_seidel (const t_matrix* m,
                      const t_array* b, t_array* v,
                      double tol, int max_iter);

//################################################################################
// (functional) analysis

// Type defintion of function of scalar 1d function with void pointer
typedef double STD_FUNC (double, void*);

// selection of standard functions

/*----recursive definition of factorial----*/
long int tn_factorial (long int n);

//--------------------------------------------------------------------------------
// algorithms for solving equations

typedef struct {
    double x1;
    double x2;
} Tuple;

/*-----real solution of quadratic equation ax^2 + bx + c = 0 -----
 * with 2-Tuple as return value-----*/
Tuple tn_solve_quadratic_real (double a, double b, double c);

/*------finds root/zero of function func------
 * arguments: function pointer, initial guess, step size, tolerance between steps,
 * max number of iterations, wanted closeness of func (x_sol) to 0 */
double tn_find_root (STD_FUNC func, double x0,
                     double dx, double tol, 
                     int max_iter, double close,
                     void *params);

//--------------------------------------------------------------------------------
// numerical integration

/*------numerical integration of scalar 1d function by Midpoint method------
 * arguments: function pointer, lower limit, upper limit, step size, additional
 * parameter of integrand */
double tn_integrate_midpoint (STD_FUNC integrand,
                              double a, double b,
                              double dx, void *params);

/*------numerical integration of scalar 1d function by Simpson------
 * same arguments as in midpoint method */
double tn_integrate_simpson (STD_FUNC integrand,
                             double a, double b,
                             double dx, void *params);

/*--fourier-transform of 1 dimensional scalar function integrand(t)--
 * arguments: integrand, limit --> the higher the more precise of fourier trafo,
 * wave number at which fourier trafo is evaluated, time step,
 * additional parameters */
double complex tn_fourier_transform (double complex integrand(double, void *),
                                     double m, double k, double dt, 
                                     void *params);

/*------differentiation according to Stirling------*/
double tn_diff_1 (STD_FUNC func, double x, double dx, void *params);

/*------second derivative------*/
double tn_diff_2 (STD_FUNC func, double x, double dx, void *params);

//################################################################################
// differential equations

/*--Type defintion of function of ordinary differential equation without dim--*/
typedef void ODE_FUNC (double, const double[], double[], void*);

/*------euler-stepper------*/
// err ~ O(dt)
void tn_euler_step (double t, double dt,
                    double* y, ODE_FUNC ode_func,
                    int dim, void *params);

/*------runge-kutta-2-stepper------*/
// err ~ O(dt^2)
void tn_rk2_step (double t, double dt,
                  double* y, ODE_FUNC ode_func,
                  int dim, void *params);

/*------runge-kutta-4-stepper------*/
// err ~ O(dt^4)
void tn_rk4_step (double t, double dt,
                  double* y, double* kArray,
                  ODE_FUNC ode_func, int dim,
                  void *params);

/*------velocity-verlet-stepper------*/
// err ~ O(dt^2)
void tn_vv_step (double t, double dt,
                 double* y, ODE_FUNC ode_func,
                 int dim, void *params);

//################################################################################
// stochastics

int tn_binomialCoeff (const unsigned int n, const unsigned int k);

// probability distributions
double tn_normal_distribution (double x, void *params);
double tn_cumulative_distr (double x, void *params);
double tn_binomial_distribution (int k, const unsigned int n, const double p);

/* Generate an array of len n of random numbers from a normal distribution */
t_array* tn_rand_alloc (int n, double sigma, int seed);

#endif

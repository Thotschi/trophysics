#include "../include/t_numerics.h"
#include "../include/t_programcontrol.h"

// debug makro --> set to 1 to print debug messages
#ifndef DEBUG
#define DEBUG 0
#endif

struct t_array {
    double* ptr;
    size_t len;
    size_t refcnt;        // reference counter
};

struct t_matrix {
    size_t rows;
    size_t cols;         
    t_array* data;        // contiguous memory block
    gsl_matrix view;      // direct GSL matrix (view)
    // flag if view was updated after altered pointer
    int cache_valid;      
    size_t refcnt;        // reference counter
};

static inline void build_cache (t_matrix* m);

static inline double
tn_dot_prod_ptr(const double* x, const double* y, size_t len);

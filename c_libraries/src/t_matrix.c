#include "t_numerics_intern.h"

//================================================================================
//    matrices
//================================================================================

//-----------------------------------
// constructor and destructor (unref)
//-----------------------------------

t_matrix*
t_matrix_alloc(size_t rows, size_t cols)
{
    if (rows == 0 || cols == 0) {
        tp_raiseError("Neither rows nor columns can be zero!");
    }
    t_matrix* m = malloc(sizeof(t_matrix));
    Null_exit_message(m, "Memory allocation failed in alloc_t_matrix!");

    m->rows = rows;
    m->cols = cols;

    // allocate large block
    m->data = t_array_alloc(rows * cols);
    Null_exit_message(m->data, "Memory allocation failed in alloc_t_matrix!");

    m->data->len = rows * cols;
    m->data->refcnt = 1;

    // create GSL view, sharing same memory
    gsl_matrix_view v = gsl_matrix_view_array(m->data->ptr, rows, cols);
    // copied into heap memory
    m->view = v.matrix;

    m->refcnt = 1;
    m->cache_valid = 1;

    return m;
}

t_matrix*
t_matrix_create_from_gsl_matrix (const gsl_matrix* src)
{
    if (!src) {
        tp_raiseError("Null pointer in t_matrix_create_from_gsl_matrix.");
    }
    t_matrix* m = t_matrix_alloc(src->size1, src->size2);
    t_matrix_copy_from_gsl_matrix(m, src);
    gsl_matrix_view v = gsl_matrix_view_array(m->data->ptr, m->rows, m->cols);
    m->view = v.matrix;
    return m;
}

void
t_matrix_ref (t_matrix* m)
{
    if (m) {
        m->refcnt++;
    }
}

void
t_matrix_unref (t_matrix* m)
{
    if (m) {
        if (--m->refcnt == 0) {
            #if DEBUG == 1
            printf("> Freeing matrix at %p\n", (void*)m);
            #endif
            t_array_unref(m->data);
            free(m);
        }
    }
}

//-----------------------------------
// setter and getter
//-----------------------------------

double
t_matrix_get (t_matrix* m, size_t i, size_t j)
{   
    if (!m) {
        tp_raiseError("Null pointer in t_matrix_get.");
    }
    if (i >= m->rows || j >= m->cols) {
        tp_raiseError("Index out of bounds in t_matrix_get.");
    }
    return m->data->ptr[i * m->cols + j];
}

void
t_matrix_set(t_matrix* m, size_t i, size_t j, double value)
{   
    if (!m) {
        tp_raiseError("Null pointer in t_matrix_set.");
    }
    if (i >= m->rows || j >= m->cols) {
        tp_raiseError("Index out of bounds in t_matrix_set.");
    }
    m->data->ptr[i * m->cols + j] = value;
}

//-----------------------------------
// copy functions (shallow)
//-----------------------------------

static inline void /* helper function (--> non public) */
build_cache (t_matrix* m)
{
    if (m->cache_valid) return;
    gsl_matrix_view v = gsl_matrix_view_array(m->data->ptr, m->rows, m->cols);
    m->view = v.matrix;
    m->cache_valid = 1;
}

void
t_matrix_assign_t_array (t_matrix* m, t_array* heap_ptr)
{   
    /* Because user can only intialize t_array via api-function
     * which allocates memory on heap and because this function
     * only accepts pointer on t_array we can garantee that
     * only a heap pointer is written into data.
     * Otherwise we would receive massive problems during runtime. */
    if (!m || !heap_ptr) {
        tp_raiseError("Null pointer in t_matrix_copy_from_heap.");
    }
    if (m->rows*m->cols != heap_ptr->len) {
        tp_raiseError("Incompatible array size in t_matrix_copy_from_heap.");
    }
    t_array_unref(m->data);
    m->data = heap_ptr;
    t_array_ref(heap_ptr);
    m->cache_valid = 0;
    build_cache (m);
}

gsl_matrix*
t_matrix_get_gsl_matrix(t_matrix* m)
{
    if (!m) {
        tp_raiseError("Null pointer in t_matrix_get_gsl_matrix.");
    }
    return &m->view;
}

//-----------------------------------
// copy functions (deep)
//-----------------------------------

void
t_matrix_copy_from_array (t_matrix* m, const double* ptr)
{   
    if (!m || !ptr) {
        tp_raiseError("Null pointer in t_matrix_copy_from_array.");
    }
    memcpy(m->data->ptr, ptr, m->rows * m->cols * sizeof(double));
}

void
t_matrix_copy_from_t_array (t_matrix* m, const t_array* arr)
{
    if (!m || !arr) {
        tp_raiseError("Null pointer in t_matrix_copy_from_t_array.");
    }
    if (m->rows * m->cols != arr->len) {
        tp_raiseError("Incompatible array size in t_matrix_copy_from_t_array.");
    }
    memcpy(m->data->ptr, arr->ptr, m->rows * m->cols * sizeof(double));
}

void
t_matrix_copy (t_matrix* dest, const t_matrix* src)
{
    if (!dest || !src) {
        tp_raiseError("Null pointer in t_matrix_copy.");
    }
    if (dest->rows != src->rows || dest->cols != src->cols) {
        tp_raiseError("Incompatible matrix size in t_matrix_copy.");
    }
    memcpy(dest->data->ptr, src->data->ptr, src->rows * src->cols * sizeof(double));
}

void
t_matrix_copy_from_gsl_matrix (t_matrix* m, const gsl_matrix* src)
{
    if (!m || !src) {
        tp_raiseError("Null pointer in t_matrix_copy_from_gsl_matrix.");
    }
    if (m->rows != src->size1 || m->cols != src->size2) {
        tp_raiseError("Incompatible matrix size in t_matrix_copy_from_gsl_matrix.");
    }
    memcpy(m->data->ptr, src->data, m->rows * m->cols * sizeof(double));
}

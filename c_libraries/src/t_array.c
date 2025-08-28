#include "t_numerics_intern.h"

//================================================================================
//    arrays
//================================================================================

//-----------------------------------
// constructor and destructor (unref)
//-----------------------------------

t_array*
t_array_alloc (size_t len)
{
    t_array* arr = malloc(sizeof(t_array));
    Null_exit_message(arr, "Memory allocation failed in t_array_alloc!");
    arr->ptr = (double*) calloc(len, sizeof(double));
    Null_exit_message(arr->ptr, "Memory allocation failed in t_array_alloc!");
    arr->len = len;
    arr->refcnt = 1;
    return arr;
}

void
t_array_ref(t_array* arr)
{
    if (arr) {
        arr->refcnt++;
    }
}

void /* can be replaced be macro FREE_T_ARRAY */
t_array_unref (t_array* arr)
{
    if (arr) {
        if (--arr->refcnt == 0) {
            #if DEBUG == 1
            printf("> Freeing array at %p\n", (void*)arr);
            #endif
            free(arr->ptr);
            free(arr);
        }
    }
}

//-----------------------------------
// getter and setter
//-----------------------------------

double
t_array_get (t_array* arr, size_t index)
{
    if (!arr) {
        tp_raiseError("Invalid array in t_array_get.");
    }
    if (index >= arr->len) {
        tp_raiseError("Invalid index in t_array_get.");
    }
    return arr->ptr[index];
}

void
t_array_set (t_array* arr, size_t index, double value)
{
    if (!arr) {
        tp_raiseError("Invalid array in t_array_set.");
    }
    if (index >= arr->len) {
        tp_raiseError("Invalid index in t_array_set.");
    }
    arr->ptr[index] = value;
}

//-----------------------------------
// copy functions (shallow and deep)
//-----------------------------------

void
t_array_copy_from_any (t_array* arr,
                            const double* data,
                            size_t len)
{   
    /* Because we cannot check location of pointer stack- as well as heap
     * pointer can only be passed to this function which copies nonetheless*/
    if (!arr || !data || len > arr->len) {
        tp_raiseError("Invalid arguments in copy_into_t_array_from_any.");
    }
    memcpy(arr->ptr, data, len * sizeof(double));
}

void
t_array_assign(t_array** dest, t_array* src) {
    if (!dest || !src) {
        tp_raiseError("NULL pointers in t_array_assign.");
    }

    t_array_unref(*dest);
    *dest = src;
    t_array_ref(src);
}

void t_array_copy_t_array(t_array* dest, const t_array* src) {
    if (!dest || !src) {
        tp_raiseError("NULL pointers in t_array_copy_t_array.");
    }
    if (dest->len != src->len) {
        tp_raiseError("Incompatible array sizes in t_array_copy_t_array.");
    }
    memcpy(dest->ptr, src->ptr, src->len * sizeof(double));
}

//-----------------------------------
// array creation functions
//-----------------------------------

t_array*
tn_linspace_alloc (double start, double stop, unsigned int n)
{
    t_array* arr = t_array_alloc(n);
    // Special case for single element
    if (n == 1) {
        tp_raiseWarning("Single element array created.");
        arr->ptr[0] = start;
        return arr;
    }
    // start and step count into n
    double step = (stop - start) / (n - 1);
    for (unsigned int i = 0; i < n; i++) {
        arr->ptr[i] = start + i * step;
    }
    return arr;
}

t_array*
tn_arange_alloc (double start, double step, unsigned int n)
{   
    t_array* arr = t_array_alloc(n);
    // Special case for single element
    if (n == 1) {
        tp_raiseWarning("Single element array created.");
        arr->ptr[0] = start;
        return arr;
    }
    for (unsigned int i = 0; i < n; i++) {
        arr->ptr[i] = start + i * step;
    }
    return arr;
}

t_array*
tn_logspace_alloc (double start, double stop, unsigned int n)
{
    t_array* arr = t_array_alloc(n);
    // Special case for single element
    if (n == 1) {
        tp_raiseWarning("Single element array created.");
        arr->ptr[0] = start;
        return arr;
    }
    double log_start = log10(start);
    double log_stop = log10(stop);
    double step = (log_stop - log_start) / (n - 1);
    for (unsigned int i = 0; i < n; i++) {
        arr->ptr[i] = pow(10, log_start + i * step);
    }
    return arr;
}

#include "t_numerics_intern.h"

//================================================================================
//    linear algebra
//================================================================================

double
tn_dot_product (const t_array* x, const t_array* y)
{   
    if (x->len != y->len) {
        tp_raiseError("Incompatible array lengths in tn_dot_product");
    }
    double sum = 0.0;
    for (size_t i = 0; i < x->len; i++) {
        sum += x->ptr[i] * y->ptr[i];
    }
    return sum;
}

static inline double
tn_dot_prod_ptr(const double* x, const double* y, size_t len)
{
    double sum = 0.0;
    for (size_t i = 0; i < len; i++) {
        sum += x[i] * y[i];
    }
    return sum;
}

void
tn_matrix_dot_vector (t_matrix* m, const t_array* v, t_array* b)
{
    double sum_row = 0.0;
    for (size_t i = 0; i < m->rows; i++) {
        sum_row = 0;
        for (size_t j = 0; j < m->cols; j++) {
            sum_row += t_matrix_get(m, i, j) * v->ptr[j];
        }
        b->ptr[i] = sum_row;
    }
}

void
tn_matrix_dot_matrix (t_matrix* a, t_matrix* b, t_matrix* c)
{
    double sum;
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += t_matrix_get(a, i, k) * t_matrix_get(b, k, j);
            }
            t_matrix_set(c, i, j, sum);
        }
    }
}

double
tn_vec_dist (t_array* y1, t_array* y2)
{
    double sum = 0.0;
    for (size_t i = 0; i < y1->len; i++) {
        sum += (y1->ptr[i] - y2->ptr[i]) * (y1->ptr[i] - y2->ptr[i]);
    }
    return sqrt(sum);
}

double
tn_len_vec_3d (t_array* y)
{
    return sqrt(tn_dot_product(y, y));
}

double
tn_len_vec (t_array* y)
{
    return sqrt(tn_dot_product(y, y));
}

double
tn_len_vec_2d (t_array* y)
{
    return sqrt(y->ptr[0] * y->ptr[0] + y->ptr[1] * y->ptr[1]);
}

void
tn_norm_vec (t_array* v)
{
    double len = tn_len_vec (v);
    for (size_t i = 0; i < v->len; i++) {
        v->ptr[i] = v->ptr[i] / len;
    }
}

void
tn_norm_vec_sum_1 (t_array* v)
{
    double len = 0.0;
    for (size_t i = 0; i < v->len; i++) {
        len += v->ptr[i];
    }
    for (size_t i = 0; i < v->len; i++) {
        v->ptr[i] = v->ptr[i] / len;
    }
}

void
tn_print_vec (t_array* v, char vec_name[])
{
    for (size_t i = 0; i < v->len; i++) {
        printf("  %s[%zu]: %g\n", vec_name, i, v->ptr[i]);
    }
}

void
tn_print_matrix (t_matrix* m)
{
    /* at corners Unicode chars U+2308 to U+230B are used
     * (gaussian brackets) */
    for (size_t i = 0; i < m->rows; i++) {
        if (i == 0) {
            printf("  ⌈\t");
        } else if (i == m->rows - 1) {
            printf("  ⌊\t");
        } else {
            printf("  |\t");
        }
        for (size_t j = 0; j < m->cols; j++) {
            printf("%g\t", t_matrix_get(m, i, j));
        }
        if (i == 0) {
            printf("⌉");
        } else if (i == m->rows - 1) {
            printf("⌋");
        } else {
            printf("|");
        }
        printf("\n");
    }
}

//--------------------------------------------------------------------------------
// algorithm to solve system of linear equations

void
tn_gauss_seidel_step (const t_matrix* m,
                      const t_array* b,
                      t_array* v)
{
    // idea for algorithm: https://youtu.be/zf2-YCo2qfU?si=7UzeM_o9JpRcizW7&t=292
    for(size_t i = 0; i < m->rows; i++){
        // a[i] is i^th row of a
        v->ptr[i] += (b->ptr[i] - tn_dot_prod_ptr(m->data->ptr + i * m->cols, v->ptr, v->len)) / m->data->ptr[i * m->cols + i];
    }
}

void
tn_gauss_seidel (const t_matrix* m,
                 const t_array* b, t_array* v,
                 double tol, int max_iter)
{   
    if (m->rows != m->cols) {
        tp_raiseError("Matrix A must be quadratic for Gauß-Seidel algorithm!\n");
    }
    for (size_t i = 0; i < m->rows; i++) {
        if (m->data->ptr[i * m->cols + i] == 0) {
            tp_raiseError("At least one diagonal element of matrix A is 0. "
                "Gauß-Seidel algorithm is not applicable!\n");
        }
    }
    size_t n = m->rows;
    t_array* old_v = t_array_alloc(n);
    double err;
    /* repeat applying stepper until either maximum iteration is reached
     * or norm of difference between steps is small enough */
    for (int k = 0; k < max_iter; k++)
        {
        t_array_copy_t_array(old_v, v);
        tn_gauss_seidel_step (m, b, v);
        err = tn_vec_dist (v, old_v);
        if(err <= tol){
            printf("> Solution of Gauß-Seidel was found using %d iterations.\n", k);
            t_array_unref(old_v);
            return;
        }
    }
    tp_raiseWarning (EMPTYSTR);
    printf("Solution of Gauß-Seidel algorithm was found exhausting given "
        "maximum number of %d iterations\n  with a norm of difference between "
        "consecutive iteration steps of %g.\n  Following calculations might be "
        "faulty!\n", max_iter, err);
    t_array_unref(old_v);
    return;
}

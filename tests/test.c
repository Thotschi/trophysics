#include "../c_libraries/include/t_numerics.h"

#define copy_from_heap 1

int main(void) {
    t_matrix* m = t_matrix_alloc(3, 3);

    #if copy_from_heap == 1
    printf("Matrix is copied from heap.\n");
    t_array* data = t_array_alloc(9);
    t_array_set(data, 0, 5.0);
    t_array_set(data, 1, 2.0);
    t_array_set(data, 2, 1.0);
    t_array_set(data, 3, 1.0);
    t_array_set(data, 4, 4.0);
    t_array_set(data, 5, 1.0);
    t_array_set(data, 6, 2.0);
    t_array_set(data, 7, 1.0);
    t_array_set(data, 8, 6.0);
    t_matrix_assign_t_array(m, data);
    #endif

    double b[] = {12.0,12.0,22.0};
    t_array* b_array = t_array_alloc(3);
    t_array_copy_from_any(b_array, b, 3);

    t_array* v = t_array_alloc(3);

    tn_print_matrix(m);
    tn_print_vec(b_array, "b");
    tn_print_vec(v, "v");
    printf("%g\n", t_matrix_get(m, 1, 1));

    tn_gauss_seidel (m, b_array, v, 1e-6, 1000);
    printf("Nach Gauss-Seidel:\n");
    tn_print_vec(v, "v");

    // GSL-Funktion benutzen
    gsl_matrix_transpose(t_matrix_get_gsl_matrix(m));

    // Ausgabe über GSL
    printf("Nach GSL-Transpose:\n");
    tn_print_matrix(m);

    #if copy_from_heap == 1
    T_ARRAY_FREE(data);
    T_MATRIX_FREE(m);
    T_ARRAY_FREE(b_array);
    T_ARRAY_FREE(v);
    #endif
    
    return 0;
}

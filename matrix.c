#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/*
 * Generates a random double between `low` and `high`.
 */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/*
 * Generates a random matrix with `seed`.
 */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocate space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. Remember to set all fieds of the matrix struct.
 * `parent` should be set to NULL to indicate that this matrix is not a slice.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. If you don't set python error messages here upon
 * failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */
 /*
 typedef struct matrix {
  int rows;       // number of rows
  int cols;       // number of columns
  double **data;  // each element is a pointer to a row of data
  int is_1d;      // Whether this matrix is a 1d matrix
                  // For 1D matrix, shape is (rows * cols)
  int ref_cnt;
  struct matrix *parent;
} matrix;
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    if (rows < 1 || cols < 1) {
        return -1;
    }
    *mat = malloc(sizeof(matrix));
    if (!*mat) {
        return -1;
    }
    (*mat)->rows = rows;
    (*mat)->cols = cols;
    (*mat)->ref_cnt = 1;
    (*mat)->parent = NULL;
    (*mat)->is_1d = rows * cols;
    (*mat)->data = (double **) malloc(rows * sizeof(double *));
    if (!(*mat)->data) {
        free(mat);
        return -1;
    }
    for (int i = 0; i < (*mat)->rows; i++) {
        double *curr_row = (*mat)->data[i] = calloc((*mat)->cols, sizeof(double));
        if (!curr_row) {
            for (int x = 0; x < i; x++){
                free((*mat)->data[x]);
            }
            free((*mat)->data);
            free(*mat);
            return -1;
        }
    }
    return 0;
}

/*
 * Allocate space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * This is equivalent to setting the new matrix to be
 * from[row_offset:row_offset + rows, col_offset:col_offset + cols]
 * If you don't set python error messages here upon failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int row_offset, int col_offset,
                        int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (row_offset >= rows || col_offset >= cols) {
        return -1;;
    }
    (*mat).parent = from;
    allocate_matrix(mat, rows, cols);
    (*mat)->ref_cnt = from->ref_cnt + 1;
    from->ref_cnt = from->ref_cnt + 1;
    for (int r = row_offset; r < (*mat)->rows; r++) {
        for (int c = col_offset; c < (*mat)->cols; c++) {
            (*mat)->data[r - row_offset][c - col_offset] = from->data[r][c];
        }
    }
    return 0;
}

/*
 * This function will be called automatically by Python when a numc matrix loses all of its
 * reference pointers.
 * You need to make sure that you only free `mat->data` if no other existing matrices are also
 * referring this data array.
 * See the spec for more information.
 */
void deallocate_matrix(matrix *mat) {
    if (!mat) {
        return -1;
    }
    if(mat->ref_cnt == 1){
        for (int r = 0; r < mat->rows; r++) {
            free(mat->data[r]);
        }
        free(mat->data);
    }
    free(mat);
    return 0;
}

/*
 * Return the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col) {
    return mat->data[row][col];
}

/*
 * Set the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val) {
    mat->data[row][col] = val;
}

/*
 * Set all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    for (int r = 0; r < mat->rows; r++) {
        for(int c = 0; c < mat->cols; c++){
          mat->data[r][c] = val;
        }
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        return -1;
    }
    for (int r = 0; r < mat1->rows; r++) {
        for(int c = 0; c < mat1->cols; c++){
            result->data[r][c] = mat1->data[r][c] + mat2->data[r][c];
        }
    }
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        return -1;
    }
    for (int r = 0; r < mat1->rows; r++) {
        for(int c = 0; c < mat1->cols; c++){
            result->data[r][c] = mat1->data[r][c] - mat2->data[r][c];
        }
    }
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1->cols != mat2->rows) {
        return -1;
    }
    // AB = C A = 4 * 3 B = 3 * 2 C = 4 * 2
    for (int r = 0; r < mat1->rows; r++) {
        for(int c = 0; c < mat2->cols; c++){
            int temp = 0;
            for(int i = 0; i < mat1->cols; i++) {
                temp = mat1->data[r][i] * mat2->data[i][c] + temp;
                result->data[r][c] = temp;
            }
        }

    }
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    if (mat->rows != mat->cols || pow < 0) {
        return -1;
    }
    if (pow == 0) {
        for (int r = 0; r < mat->rows; r++) {
            for (int c = 0; c < mat->cols; c++) {
                if(c == r){
                    result->data[r][c] = 1;
                }else{
                    result->data[r][c] = 0;
                }
            }
        }
        return 0;
    } else if(pow == 1) {
        for (int r = 0; r < mat->rows; r++) {
            for (int c = 0; c < mat->cols; c++) {
                result->data[r][c] = mat->data[r][c];
            }
        }
    } else {
        mul_matrix(result, mat, mat);
        for (int i = 2; i < pow; i++) {
            mul_matrix(result, result, mat);
        }
    }
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    for (int r = 0; r < mat->rows; r++) {
        for (int c = 0; c < mat->cols; c++) {
          result->data[r][c] = -1 * mat->data[r][c];
        }
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    for (int r = 0; r < mat->rows; r++) {
        for (int c = 0; c < mat->cols; c++) {
          if(mat->data[r][c] < 0){
            result->data[r][c] = -1 * mat->data[r][c];
          }else{
            result->data[r][c] = mat->data[r][c];
          }
        }
    }
    return 0;
}

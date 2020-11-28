#include "numc.h"
#include <structmember.h>

PyTypeObject Matrix61cType;

/* Helper functions for initalization of matrices and vectors */

/*
 * Return a tuple given rows and cols
 */
PyObject *get_shape(int rows, int cols) {
  if (rows == 1 || cols == 1) {
    return PyTuple_Pack(1, PyLong_FromLong(rows * cols));
  } else {
    return PyTuple_Pack(2, PyLong_FromLong(rows), PyLong_FromLong(cols));
  }
}
/*
 * Matrix(rows, cols, low, high). Fill a matrix random double values
 */
int init_rand(PyObject *self, int rows, int cols, unsigned int seed, double low,
              double high) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    rand_matrix(new_mat, seed, low, high);
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(rows, cols, val). Fill a matrix of dimension rows * cols with val
 */
int init_fill(PyObject *self, int rows, int cols, double val) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed)
        return alloc_failed;
    else {
        fill_matrix(new_mat, val);
        ((Matrix61c *)self)->mat = new_mat;
        ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    }
    return 0;
}

/*
 * Matrix(rows, cols, 1d_list). Fill a matrix with dimension rows * cols with 1d_list values
 */
int init_1d(PyObject *self, int rows, int cols, PyObject *lst) {
    if (rows * cols != PyList_Size(lst)) {
        PyErr_SetString(PyExc_ValueError, "Incorrect number of elements in list");
        return -1;
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j, PyFloat_AsDouble(PyList_GetItem(lst, count)));
            count++;
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(2d_list). Fill a matrix with dimension len(2d_list) * len(2d_list[0])
 */
int init_2d(PyObject *self, PyObject *lst) {
    int rows = PyList_Size(lst);
    if (rows == 0) {
        PyErr_SetString(PyExc_ValueError,
                        "Cannot initialize numc.Matrix with an empty list");
        return -1;
    }
    int cols;
    if (!PyList_Check(PyList_GetItem(lst, 0))) {
        PyErr_SetString(PyExc_ValueError, "List values not valid");
        return -1;
    } else {
        cols = PyList_Size(PyList_GetItem(lst, 0));
    }
    for (int i = 0; i < rows; i++) {
        if (!PyList_Check(PyList_GetItem(lst, i)) ||
                PyList_Size(PyList_GetItem(lst, i)) != cols) {
            PyErr_SetString(PyExc_ValueError, "List values not valid");
            return -1;
        }
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j,
                PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(lst, i), j)));
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * This deallocation function is called when reference count is 0
 */
void Matrix61c_dealloc(Matrix61c *self) {
    deallocate_matrix(self->mat);
    Py_TYPE(self)->tp_free(self);
}

/* For immutable types all initializations should take place in tp_new */
PyObject *Matrix61c_new(PyTypeObject *type, PyObject *args,
                        PyObject *kwds) {
    /* size of allocated memory is tp_basicsize + nitems*tp_itemsize*/
    Matrix61c *self = (Matrix61c *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

/*
 * This matrix61c type is mutable, so needs init function. Return 0 on success otherwise -1
 */
int Matrix61c_init(PyObject *self, PyObject *args, PyObject *kwds) {
    /* Generate random matrices */
    if (kwds != NULL) {
        PyObject *rand = PyDict_GetItemString(kwds, "rand");
        if (!rand) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (!PyBool_Check(rand)) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (rand != Py_True) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        PyObject *low = PyDict_GetItemString(kwds, "low");
        PyObject *high = PyDict_GetItemString(kwds, "high");
        PyObject *seed = PyDict_GetItemString(kwds, "seed");
        double double_low = 0;
        double double_high = 1;
        unsigned int unsigned_seed = 0;

        if (low) {
            if (PyFloat_Check(low)) {
                double_low = PyFloat_AsDouble(low);
            } else if (PyLong_Check(low)) {
                double_low = PyLong_AsLong(low);
            }
        }

        if (high) {
            if (PyFloat_Check(high)) {
                double_high = PyFloat_AsDouble(high);
            } else if (PyLong_Check(high)) {
                double_high = PyLong_AsLong(high);
            }
        }

        if (double_low >= double_high) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        // Set seed if argument exists
        if (seed) {
            if (PyLong_Check(seed)) {
                unsigned_seed = PyLong_AsUnsignedLong(seed);
            }
        }

        PyObject *rows = NULL;
        PyObject *cols = NULL;
        if (PyArg_UnpackTuple(args, "args", 2, 2, &rows, &cols)) {
            if (rows && cols && PyLong_Check(rows) && PyLong_Check(cols)) {
                return init_rand(self, PyLong_AsLong(rows), PyLong_AsLong(cols), unsigned_seed, double_low,
                                 double_high);
            }
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    }
    PyObject *arg1 = NULL;
    PyObject *arg2 = NULL;
    PyObject *arg3 = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 3, &arg1, &arg2, &arg3)) {
        /* arguments are (rows, cols, val) */
        if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && (PyLong_Check(arg3)
                || PyFloat_Check(arg3))) {
            if (PyLong_Check(arg3)) {
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyLong_AsLong(arg3));
            } else
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyFloat_AsDouble(arg3));
        } else if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && PyList_Check(arg3)) {
            /* Matrix(rows, cols, 1D list) */
            return init_1d(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), arg3);
        } else if (arg1 && PyList_Check(arg1) && arg2 == NULL && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_2d(self, arg1);
        } else if (arg1 && arg2 && PyLong_Check(arg1) && PyLong_Check(arg2) && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), 0);
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return -1;
    }
}

/*
 * List of lists representations for matrices
 */
PyObject *Matrix61c_to_list(Matrix61c *self) {
    int rows = self->mat->rows;
    int cols = self->mat->cols;
    PyObject *py_lst = NULL;
    if (self->mat->is_1d) {  // If 1D matrix, print as a single list
        py_lst = PyList_New(rows * cols);
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(py_lst, count, PyFloat_FromDouble(get(self->mat, i, j)));
                count++;
            }
        }
    } else {  // if 2D, print as nested list
        py_lst = PyList_New(rows);
        for (int i = 0; i < rows; i++) {
            PyList_SetItem(py_lst, i, PyList_New(cols));
            PyObject *curr_row = PyList_GetItem(py_lst, i);
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(curr_row, j, PyFloat_FromDouble(get(self->mat, i, j)));
            }
        }
    }
    return py_lst;
}

PyObject *Matrix61c_class_to_list(Matrix61c *self, PyObject *args) {
    PyObject *mat = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 1, &mat)) {
        if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
            return NULL;
        }
        Matrix61c* mat61c = (Matrix61c*)mat;
        return Matrix61c_to_list(mat61c);
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }
}

/*
 * Add class methods
 */
PyMethodDef Matrix61c_class_methods[] = {
    {"to_list", (PyCFunction)Matrix61c_class_to_list, METH_VARARGS, "Returns a list representation of numc.Matrix"},
    {NULL, NULL, 0, NULL}
};

/*
 * Matrix61c string representation. For printing purposes.
 */
PyObject *Matrix61c_repr(PyObject *self) {
    PyObject *py_lst = Matrix61c_to_list((Matrix61c *)self);
    return PyObject_Repr(py_lst);
}

/* NUMBER METHODS */

/*
 * Add the second numc.Matrix (Matrix61c) object to the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_add(Matrix61c* self, PyObject* args) {
    if (!PyObject_TypeCheck(args, &Matrix61cType)){
        PyErr_SetString(PyExc_TypeError, "Argument must have type numc.Matrix!");
        return NULL;
    }
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    int rowsB = ((Matrix61c *) args)->mat->rows;
    int colsB = ((Matrix61c *) args)->mat->cols;
    if(rowsA != rowsB || colsA != colsB){
        PyErr_SetString(PyExc_ValueError, "Argument must have same dimensions!");
        return NULL;
    }
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsA);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    add_matrix(result, self->mat, ((Matrix61c *) args)->mat);
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * Substract the second numc.Matrix (Matrix61c) object from the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_sub(Matrix61c* self, PyObject* args) {
    if (!PyObject_TypeCheck(args, &Matrix61cType)){
        PyErr_SetString(PyExc_TypeError, "Argument must have type numc.Matrix!");
        return NULL;
    }
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    int rowsB = ((Matrix61c *) args)->mat->rows;
    int colsB = ((Matrix61c *) args)->mat->cols;
    if(rowsA != rowsB || colsA != colsB){
        PyErr_SetString(PyExc_ValueError, "Argument must have same dimensions!");
        return NULL;
    }
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsA);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    sub_matrix(result, (self->mat), (((Matrix61c *) args)->mat));
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * NOT element-wise multiplication. The first operand is self, and the second operand
 * can be obtained by casting `args`.
 */
PyObject *Matrix61c_multiply(Matrix61c* self, PyObject *args) {
    if (!PyObject_TypeCheck(args, &Matrix61cType)){
        PyErr_SetString(PyExc_TypeError, "Argument must have type numc.Matrix!");
        return NULL;
    }
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    int rowsB = ((Matrix61c *) args)->mat->rows;
    int colsB = ((Matrix61c *) args)->mat->cols;
    if(colsA != rowsB){
        PyErr_SetString(PyExc_ValueError, "Argument must have same dimensions!");
        return NULL;
    }
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsB);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    mul_matrix(result, self->mat, ((Matrix61c *) args)->mat);
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * Negates the given numc.Matrix.
 */
PyObject *Matrix61c_neg(Matrix61c* self) {
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsA);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    neg_matrix(result, self->mat);
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * Take the element-wise absolute value of this numc.Matrix.
 */
PyObject *Matrix61c_abs(Matrix61c *self) {
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsA);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    abs_matrix(result, self->mat);
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * Raise numc.Matrix (Matrix61c) to the `pow`th power. You can ignore the argument `optional`.
 * TypeError if pow is not an integer.
 * ValueError if a is not a square matrix or if pow is negative.
 */
PyObject *Matrix61c_pow(Matrix61c *self, PyObject *pow, PyObject *optional) {
    if (!PyLong_Check(pow)){
        PyErr_SetString(PyExc_TypeError, "Argument must have type int!");
        return NULL;
    }
    int rowsA = self->mat->rows;
    int colsA = self->mat->cols;
    if(rowsA != colsA || (long) pow < 0){
        PyErr_SetString(PyExc_ValueError, "Argument must have be square, or pow must be positive!");
        return NULL;
    }
    matrix *result;
    int alloc_failed = allocate_matrix(&result, rowsA, colsA);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
        return NULL;
    }
    pow_matrix(result, self->mat, (long) pow);
    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    rv->mat = result;
    rv->shape = get_shape(result->rows, result->cols);
    return (PyObject *) rv;
}

/*
 * Create a PyNumberMethods struct for overloading operators with all the number methods you have
 * define. You might find this link helpful: https://docs.python.org/3.6/c-api/typeobj.html
 */
PyNumberMethods Matrix61c_as_number = {
    .nb_add = (binaryfunc) Matrix61c_add,
    .nb_subtract = (binaryfunc) Matrix61c_sub,
    .nb_multiply = (binaryfunc) Matrix61c_multiply,
    .nb_negative = (unaryfunc) Matrix61c_neg,
    .nb_absolute = (unaryfunc) Matrix61c_abs,
    .nb_power = (ternaryfunc) Matrix61c_pow
};


/* INSTANCE METHODS */

/*
 * Given a numc.Matrix self, parse `args` to (int) row, (int) col, and (double/int) val.
 * Return None in Python (this is different from returning null).
 */
PyObject *Matrix61c_set_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */
    int i;
    int j;
    double val;
    if (!PyArg_ParseTuple(args, "iid", &i, &j, &val)) {
        PyErr_SetString(PyExc_TypeError, "Either number of arguments is != 3; i, j are not integers; or val is not a double, int.");
        return NULL;
    }
    if(i >= self->mat->rows || j >= self->mat->cols){
        PyErr_SetString(PyExc_IndexError, "Index out of bounds!");
        return NULL;
    }
    set(self->mat, i, j, val);
    return Py_None;
}

/*
 * Given a numc.Matrix `self`, parse `args` to (int) row and (int) col.
 * Return the value at the `row`th row and `col`th column, which is a Python
 * float/int.
 */
PyObject *Matrix61c_get_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */
    int i;
    int j;
    if (!PyArg_ParseTuple(args, "ii", &i, &j)) {
        PyErr_SetString(PyExc_TypeError, "Either number of arguments is != 2; i, j are not integers.");
        return NULL;
    }
    if(i >= self->mat->rows || j >= self->mat->cols){
        PyErr_SetString(PyExc_IndexError, "Index out of bounds!");
        return NULL;
    }
    double res = get(self->mat, i, j);
    return (PyObject *) PyFloat_FromDouble(res);
}

/*
 * Create an array of PyMethodDef structs to hold the instance methods.
 * Name the python function corresponding to Matrix61c_get_value as "get" and Matrix61c_set_value
 * as "set"
 * You might find this link helpful: https://docs.python.org/3.6/c-api/structures.html
 */
PyMethodDef Matrix61c_methods[] = {
    /* TODO: YOUR CODE HERE */
    {"set", (PyCFunction)Matrix61c_set_value, METH_VARARGS, "Sets the value of a (i,j) of a matrix"},
    {"get", (PyCFunction)Matrix61c_get_value, METH_VARARGS, "Gets the value of a (i,j) of a matrix"},
    {NULL, NULL, 0, NULL}
};

/* INDEXING */

/*
 * Given a numc.Matrix `self`, index into it with `key`. Return the indexed result.
 */
PyObject *Matrix61c_subscript(Matrix61c* self, PyObject* key) {
    int rows = self->mat->rows;
    int cols = self->mat->cols;
    if (rows == 1 || cols == 1) {
        if (PyLong_Check(key)) {
            // FIXED, BUT CHECK NEGATIVE INDICIES
            if (rows == 1) {
                int out = get(self->mat, 0, PyLong_AsLong(key));
                return (PyObject *) PyLong_FromLong(((long) out));
            } else if (cols == 1){
                int out = get(self->mat, PyLong_AsLong(key), 0);
                return (PyObject *) PyLong_FromLong(((long) out));
            }
        }else if (PySlice_Check(key)) {
            Py_ssize_t start1;
            Py_ssize_t stop1;
            Py_ssize_t step1;
            Py_ssize_t sliceLength1;
            matrix *mat;
            int alloc_failed = 0;
            if (rows == 1) {
                if(PySlice_GetIndicesEx(key, cols, &start1, &stop1, &step1, &sliceLength1)) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                }
                if(step1 != 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                } else if(sliceLength1 < 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                }
                if (sliceLength1 == 1) {
                    int out = get(self->mat, 0, (long) start1);
                    return (PyObject *) PyLong_FromLong(((long) out));
                }
                alloc_failed = allocate_matrix_ref(&mat, self->mat, 0, start1, 1, stop1 - start1);
            } else if (cols == 1) {
                if(PySlice_GetIndicesEx(key, rows, &start1, &stop1, &step1, &sliceLength1)) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                }
                if(step1 != 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                } else if(sliceLength1 < 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return NULL;
                }
                if (sliceLength1 == 1) {
                    int out = get(self->mat, (long) start1, 0);
                    return (PyObject *) PyLong_FromLong(((long) out));
                }
                alloc_failed = allocate_matrix_ref(&mat, self->mat, start1, 0, stop1 - start1, 1);
            }
            if (alloc_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                return NULL;
            }
            Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
            rv->mat = mat;
            rv->shape = get_shape(mat->rows, mat->cols);
            return (PyObject *) rv;
        } else {
            PyErr_SetString(PyExc_TypeError, "Key is not an integer, or slice!");
            return NULL;
        }
    } else {
        if (PyLong_Check(key)) {
            matrix *mat;
            // FIXED BY JAI
            int alloc_failed = allocate_matrix_ref(&mat, self->mat, (int) PyLong_AsLong(key), 0, 1, cols);
            if (alloc_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                return NULL;
            }
            Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
            rv->mat = mat;
            rv->shape = get_shape(mat->rows, mat->cols);
            return (PyObject *) rv;
        }else if (PySlice_Check(key)) {
            Py_ssize_t start1;
            Py_ssize_t stop1;
            Py_ssize_t step1;
            Py_ssize_t sliceLength1;
            if(PySlice_GetIndicesEx(key, rows, &start1, &stop1, &step1, &sliceLength1)) {
                PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                return NULL;
            }
            if(step1 != 1) {
                PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                return NULL;
            } else if(sliceLength1 < 1) {
                PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                return NULL;
            }
            matrix *mat;
            int alloc_failed = allocate_matrix_ref(&mat, self->mat, (long)start1, 0, (long)stop1 - (long)start1, cols);
            if (alloc_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                return NULL;
            }
            Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
            rv->mat = mat;
            rv->shape = get_shape(mat->rows, mat->cols);
            return (PyObject *) rv;
        } else if (PyTuple_Check(key)) {
            PyObject* index1 = NULL;
            PyObject* index2 = NULL;
            PyArg_UnpackTuple(key, "args", 2, 2, &index1, &index2);
            if (PyLong_Check(index1)) {
                if (PyLong_Check(index2)) {
                    long out;
                    out = get(self->mat, PyLong_AsLong(index1), PyLong_AsLong(index2));
                    return (PyObject *) PyLong_FromLong((long) out);
                }else if (PySlice_Check(index2)) {
                    Py_ssize_t start2;
                    Py_ssize_t stop2;
                    Py_ssize_t step2;
                    Py_ssize_t sliceLength2;
                    if(PySlice_GetIndicesEx(index2, cols, &start2, &stop2, &step2, &sliceLength2)) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if(step2 != 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    } else if(sliceLength2 < 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if (sliceLength2 == 1) {
                        int out = get(self->mat, PyLong_AsLong(index1), (long) start2);
                        return (PyObject *) PyLong_FromLong(((long) out));
                    }
                    matrix *mat;
                    int alloc_failed = allocate_matrix_ref(&mat, self->mat, PyLong_AsLong(index1), (long)start2, 1, (long)stop2 - (long)start2);
                    if (alloc_failed) {
                        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                        return NULL;
                    }
                    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
                    rv->mat = mat;
                    rv->shape = get_shape(mat->rows, mat->cols);
                    return (PyObject *) rv;

                }
            } else if (PySlice_Check(index1)) {
                if (PyLong_Check(index2)) {
                    Py_ssize_t start1;
                    Py_ssize_t stop1;
                    Py_ssize_t step1;
                    Py_ssize_t sliceLength1;
                    if(PySlice_GetIndicesEx(index1, rows, &start1, &stop1, &step1, &sliceLength1)) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if(step1 != 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    } else if(sliceLength1 < 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if (sliceLength1 == 1) {
                        int out = get(self->mat, (long) start1, PyLong_AsLong(index2));
                        return (PyObject *) PyLong_FromLong(((long) out));
                    }
                    matrix *mat;
                    int alloc_failed = allocate_matrix_ref(&mat, self->mat, (long)start1, PyLong_AsLong(index2), (long)stop1 - (long)start1, 1);
                    if (alloc_failed) {
                        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                        return NULL;
                    }
                    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
                    rv->mat = mat;
                    rv->shape = get_shape(mat->rows, mat->cols);
                    return (PyObject *) rv;
                } else if (PySlice_Check(index2)) {
                    Py_ssize_t start1;
                    Py_ssize_t stop1;
                    Py_ssize_t step1;
                    Py_ssize_t sliceLength1;
                    Py_ssize_t start2;
                    Py_ssize_t stop2;
                    Py_ssize_t step2;
                    Py_ssize_t sliceLength2;
                    if(PySlice_GetIndicesEx(index1, rows, &start1, &stop1, &step1, &sliceLength1) ||
                        PySlice_GetIndicesEx(index2, cols, &start2, &stop2, &step2, &sliceLength2)) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if(step1 != 1 || step2 != 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    } else if(sliceLength1 < 1 || sliceLength2 < 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return NULL;
                    }
                    if (sliceLength1 == 1 && sliceLength2 == 1) {
                        int out = get(self->mat, (long) start1, (long) start2);
                        return (PyObject *) PyLong_FromLong(((long) out));
                    }
                    matrix *mat;
                    int alloc_failed = allocate_matrix_ref(&mat, self->mat, (long)start1, (long)start2, (long)stop1 - (long)start1, (long)stop2 - (long)start2);
                    if (alloc_failed) {
                        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                        return NULL;
                    }
                    Matrix61c *rv = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
                    rv->mat = mat;
                    rv->shape = get_shape(mat->rows, mat->cols);
                    return (PyObject *) rv;
                }
            }
        } else {
            PyErr_SetString(PyExc_TypeError, "Key is not an integer, slice, or a length-2 tuple of slices/ints!");
            return NULL;
        }
    }
    return NULL;
}

/*
 * Given a numc.Matrix `self`, index into it with `key`, and set the indexed result to `v`.
 */
int Matrix61c_set_subscript(Matrix61c* self, PyObject *key, PyObject *v) {
    PyObject* temp = Matrix61c_subscript(self, key);
    if (PyLong_Check(v) || PyFloat_Check(v)) {
        if (!PyLong_Check(temp) && !PyFloat_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "Resulting slice is not 1 by 1, but v is not a list.");
            return -1;
        }
        if (PyLong_Check(key)) {
            matrix *mat;
            int alloc_failed = 0;
            if (self->mat->rows == 1) {
                alloc_failed = allocate_matrix_ref(&mat, self->mat, 0, PyLong_AsLong(key), 1, 1);
            } else if (self->mat->cols == 1) {
                alloc_failed = allocate_matrix_ref(&mat, self->mat, PyLong_AsLong(key), 0, 1, 1);
            }
            if (alloc_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                return -1;
            }
            mat->data[0][0] = PyLong_AsLong(v);
        }else if (PySlice_Check(key)) {
            Py_ssize_t start1;
            Py_ssize_t stop1;
            Py_ssize_t step1;
            Py_ssize_t sliceLength1;
            matrix *mat;
            int alloc_failed = 0;
            if (self->mat->rows == 1) {
                if(PySlice_GetIndicesEx(key, self->mat->cols, &start1, &stop1, &step1, &sliceLength1)) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                }
                if(step1 != 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                } else if(sliceLength1 < 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                }
                alloc_failed = allocate_matrix_ref(&mat, self->mat, 0, start1, 1, stop1 - start1);
            } else if (self->mat->cols == 1) {
                if(PySlice_GetIndicesEx(key, self->mat->rows, &start1, &stop1, &step1, &sliceLength1)) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                }
                if(step1 != 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                } else if(sliceLength1 < 1) {
                    PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                    return -1;
                }
                alloc_failed = allocate_matrix_ref(&mat, self->mat, start1, 0, stop1 - start1, 1);
            }
            if (alloc_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                return -1;
            }
            mat->data[0][0] = PyLong_AsLong(v);
        } else if (PyTuple_Check(key)) {
          PyObject* index1 = NULL;
          PyObject* index2 = NULL;
          PyArg_UnpackTuple(key, "args", 2, 2, &index1, &index2);
          if (PyLong_Check(index1)) {
              if (PyLong_Check(index2)) {
                  matrix *mat;
                  int alloc_failed = 0;
                  alloc_failed = allocate_matrix_ref(&mat, self->mat, PyLong_AsLong(index1), PyLong_AsLong(index2), 1, 1);
                  if (alloc_failed) {
                      PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                      return -1;
                  }
                  mat->data[0][0] = PyLong_AsLong(v);
              }else if (PySlice_Check(index2)) {
                  Py_ssize_t start2;
                  Py_ssize_t stop2;
                  Py_ssize_t step2;
                  Py_ssize_t sliceLength2;
                  matrix *mat;
                  int alloc_failed = 0;
                  if(PySlice_GetIndicesEx(index2, self->mat->cols, &start2, &stop2, &step2, &sliceLength2)) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid1!");
                      return -1;
                  }
                  if(step2 != 1) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid2!");
                      return -1;
                  } else if(sliceLength2 < 1) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid3!");
                      return -1;
                  }
                  alloc_failed = allocate_matrix_ref(&mat, self->mat, PyLong_AsLong(index1), start2, 1, stop2 - start2);
                  if (alloc_failed) {
                      PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                      return -1;
                  }
                  mat->data[0][0] = PyLong_AsLong(v);
              }
          } else if (PySlice_Check(index1)) {
              if (PyLong_Check(index2)) {
                    Py_ssize_t start1;
                    Py_ssize_t stop1;
                    Py_ssize_t step1;
                    Py_ssize_t sliceLength1;
                    matrix *mat;
                    int alloc_failed = 0;
                    if(PySlice_GetIndicesEx(index1, self->mat->rows, &start1, &stop1, &step1, &sliceLength1)) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return -1;
                    }
                    if(step1 != 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return -1;
                    } else if(sliceLength1 < 1) {
                        PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                        return -1;
                    }
                    alloc_failed = allocate_matrix_ref(&mat, self->mat, start1, PyLong_AsLong(index2), stop1 - start1, 1);
                    if (alloc_failed) {
                        PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                        return -1;
                    }
                    mat->data[0][0] = PyLong_AsLong(v);
                }else if (PySlice_Check(index2)) {
                  Py_ssize_t start1;
                  Py_ssize_t stop1;
                  Py_ssize_t step1;
                  Py_ssize_t sliceLength1;
                  Py_ssize_t start2;
                  Py_ssize_t stop2;
                  Py_ssize_t step2;
                  Py_ssize_t sliceLength2;
                  if(PySlice_GetIndicesEx(index1, self->mat->rows, &start1, &stop1, &step1, &sliceLength1) ||
                      PySlice_GetIndicesEx(index2, self->mat->cols, &start2, &stop2, &step2, &sliceLength2)) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                      return -1;
                  }
                  if(step1 != 1 || step2 != 1) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                      return -1;
                  } else if(sliceLength1 < 1 || sliceLength2 < 1) {
                      PyErr_SetString(PyExc_ValueError, "Slice info not valid!");
                      return -1;
                  }
                  matrix *mat;
                  int alloc_failed = allocate_matrix_ref(&mat, self->mat, (long)start1, (long)start2, (long)stop1 - (long)start1, (long)stop2 - (long)start2);
                  if (alloc_failed) {
                      PyErr_SetString(PyExc_RuntimeError, "Allocation failed!");
                      return -1;
                  }
                  mat->data[0][0] = PyLong_AsLong(v);
            }
          }
        }
        // a[0:1,0:1]
        // a[0,0]
        //  a[0,0:1]
        // a[0:1, 0]
    } else if (PyList_Check(v)) {
        if (PyLong_Check(temp) || PyFloat_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "Resulting slice is 1 by 1, but v is not a float or int.");
            return -1;
        }
        Matrix61c* mod_mat = ((Matrix61c *) temp);
        int row = mod_mat->mat->rows;
        int col = mod_mat->mat->cols;
        if (!mod_mat->mat->is_1d) {
            if (PyList_Size(v) != row) {
                PyErr_SetString(PyExc_ValueError, "V is of wrong size.");
                return -1;
            }
            for(int r = 0; r < row; r++){
                if (PyList_Size(PyList_GetItem(v, r)) != col) {
                    PyErr_SetString(PyExc_ValueError, "V is of wrong size.");
                    return -1;
                }
                for(int c = 0; c < col; c++){
                    if(!PyLong_Check(PyList_GetItem(PyList_GetItem(v, r), c)) && !PyFloat_Check(PyList_GetItem(PyList_GetItem(v, r), c))){
                        PyErr_SetString(PyExc_ValueError, "An element of v is not a float or int");
                        return -1;
                    }
                    mod_mat->mat->data[r][c] = PyLong_AsLong(PyList_GetItem(PyList_GetItem(v, r), c));
                }
            }
        } else if (mod_mat->mat->is_1d) {
            for(int r = 0; r < row; r++){
                for(int c = 0; c < col; c++){
                    if (row == 1)  {
                        if(!PyLong_Check(PyList_GetItem(v, c)) && !PyFloat_Check(PyList_GetItem(v, c))){
                            PyErr_SetString(PyExc_ValueError, "An element of v is not a float or int");
                            return -1;
                        } else if (PyList_Size(v) != col) {
                            PyErr_SetString(PyExc_ValueError, "V is of wrong size.");
                            return -1;
                        }
                        mod_mat->mat->data[r][c] = PyLong_AsLong(PyList_GetItem(v, c));
                    } else if (col == 1) {
                        if(!PyLong_Check(PyList_GetItem(v, c)) && !PyFloat_Check(PyList_GetItem(v, r))){
                            PyErr_SetString(PyExc_ValueError, "An element of v is not a float or int");
                            return -1;
                        } else if (PyList_Size(v) != row) {
                            PyErr_SetString(PyExc_ValueError, "V is of wrong size.");
                            return -1;
                        }
                        mod_mat->mat->data[r][c] = PyLong_AsLong(PyList_GetItem(v, r));
                    }
                }
            }
        }
    }
    return 0;
}

PyMappingMethods Matrix61c_mapping = {
    NULL,
    (binaryfunc) Matrix61c_subscript,
    (objobjargproc) Matrix61c_set_subscript,
};

/* INSTANCE ATTRIBUTES*/
PyMemberDef Matrix61c_members[] = {
    {
        "shape", T_OBJECT_EX, offsetof(Matrix61c, shape), 0,
        "(rows, cols)"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject Matrix61cType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "numc.Matrix",
    .tp_basicsize = sizeof(Matrix61c),
    .tp_dealloc = (destructor)Matrix61c_dealloc,
    .tp_repr = (reprfunc)Matrix61c_repr,
    .tp_as_number = &Matrix61c_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,
    .tp_doc = "numc.Matrix objects",
    .tp_methods = Matrix61c_methods,
    .tp_members = Matrix61c_members,
    .tp_as_mapping = &Matrix61c_mapping,
    .tp_init = (initproc)Matrix61c_init,
    .tp_new = Matrix61c_new
};


struct PyModuleDef numcmodule = {
    PyModuleDef_HEAD_INIT,
    "numc",
    "Numc matrix operations",
    -1,
    Matrix61c_class_methods
};

/* Initialize the numc module */
PyMODINIT_FUNC PyInit_numc(void) {
    PyObject* m;

    if (PyType_Ready(&Matrix61cType) < 0)
        return NULL;

    m = PyModule_Create(&numcmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&Matrix61cType);
    PyModule_AddObject(m, "Matrix", (PyObject *)&Matrix61cType);
    printf("CS61C Fall 2020 Project 4: numc imported!\n");
    fflush(stdout);
    return m;
}

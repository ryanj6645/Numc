from utils import *
from unittest import TestCase

def test_medium_add():
    dp_mat1, nc_mat1 = rand_dp_nc_matrix(1000, 1000, seed=0)
    dp_mat2, nc_mat2 = rand_dp_nc_matrix(1000, 1000, seed=1)
    nc_mat1 + nc_mat2
    is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
    print_speedup(speed_up)

def mem_test():
    def f():
        return nc.Matrix(1000, 1000, 1.58729345)

    a = f()[0:100]
    b = nc.Matrix(10000, 10000, 2.5)
    c = nc.Matrix(10000, 10000, 2.5)
    b = a[0:3, 0:3]
    b = 5
    print(a[0][0])

mem_test()
test_medium_add()

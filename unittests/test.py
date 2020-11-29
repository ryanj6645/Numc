from utils import *
from unittest import TestCase

def test_medium_add(self):
    dp_mat1, nc_mat1 = rand_dp_nc_matrix(1000, 1000, seed=0)
    dp_mat2, nc_mat2 = rand_dp_nc_matrix(1000, 1000, seed=1)
    nc_mat1 + nc_mat2
    is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
    print_speedup(speed_up)

test_medium_add

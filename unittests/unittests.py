from utils import *
from unittest import TestCase
import random

"""
For each operation, you should write tests to test  on matrices of different sizes.
Hint: use dp_mc_matrix to generate dumbpy and numc matrices with the same data and use
      cmp_dp_nc_matrix to compare the results
"""
class TestAdd(TestCase):
    def test_tiny_add(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1, 1, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 1, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_add(self):
        random.seed()
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1000, 1000, seed=int(random.random() * 10))
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1000, 1000, seed=int(random.random() * 10))
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_add(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(12000, 12000, seed=int(random.random() * 10))
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(12000, 12000, seed=int(random.random() * 10))
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestSub(TestCase):
    def test_tiny_add(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1, 1, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 1, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_sub(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1000, 1000, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1000, 1000, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_sub(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(12000, 12000, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(12000, 12000, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestAbs(TestCase):
    def test_tiny_add(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_abs(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_abs_1D(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_abs_tall(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(24, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_abs_fat(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 200, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_abs(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_abs_1D(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_abs_tall(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2400, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_abs_fat(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 2000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_abs(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(12000, 12000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_abs_1D(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(12000, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_abs_tall(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(12000, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_abs_fat(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 12000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestNeg(TestCase):
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    def test_medium_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 10000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_neg_long(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1000, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_neg_tall(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_neg_1x1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)


class TestMul2(TestCase):


class TestMul(TestCase):
    def test_tiny_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1, 1, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 1, seed=1)
        dp_mat1, dp_mat2 = -dp_mat1, -dp_mat2
        nc_mat1, nc_mat2 = -nc_mat1, -nc_mat2
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_mul_5x2(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 5, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(5, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_mul(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(200, 504, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(504, 1700, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_1D(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(200, 1, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 1700, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_mat(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1500, 1500, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1500, 1500, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)


    def test_large_mul(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2000, 2000, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2000, 2000, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_mul(self):
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1764, 1349, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1349, 20, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestPow(TestCase):
    def test_small_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        is_correct, speed_up = compute([dp_mat, 3], [nc_mat, 3], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_pow1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        is_correct, speed_up = compute([dp_mat, 0], [nc_mat, 0], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_pow2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        is_correct, speed_up = compute([dp_mat, 1], [nc_mat, 1], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        is_correct, speed_up = compute([dp_mat, 3], [nc_mat, 3], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        is_correct, speed_up = compute([dp_mat, 1], [nc_mat, 1], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_pow2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        is_correct, speed_up = compute([dp_mat, 0], [nc_mat, 0], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    # def test_large_long(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(1, 2000, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 0], [nc_mat, 0], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def test_large_tall(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(2000, 1, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 0], [nc_mat, 0], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def test_large_long2(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(1, 12000, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def test_large_tall2(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(12000, 1, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def test_large_long3(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(1, 12000, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 1], [nc_mat, 1], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_large_tall3(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(12000, 1, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 1], [nc_mat, 1], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def el_diablo(self):
    #     dp_mat, nc_mat = rand_dp_nc_matrix(12000, 7000, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 15], [nc_mat, 15], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

class TestGet(TestCase):
    def test_get(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

    def test_get_big(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 1000, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

    def test_get_1x1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(0)
        self.assertEqual(round(dp_mat[rand_row][0], decimal_places),
            round(nc_mat[rand_row][0], decimal_places))

    def test_get_tall(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 1, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(0)
        self.assertEqual(round(dp_mat[rand_row][0], decimal_places),
            round(nc_mat[rand_row][0], decimal_places))

    def test_get_long(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1000, seed=0)
        rand_row = np.random.randint(0)
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[0][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))


class TestSet(TestCase):
    def test_set(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEquals(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

    def test_set_big(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEquals(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

    def test_set_1x1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(0)
        self.assertEquals(round(dp_mat[0][0], decimal_places),
            round(nc_mat[0][0], decimal_places))

    def test_set_tall(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 1, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(0)
        self.assertEquals(round(dp_mat[rand_row][0], decimal_places),
            round(nc_mat[rand_row][0], decimal_places))

    def test_set_long(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1000, seed=0)
        rand_row = np.random.randint(0)
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEquals(round(dp_mat[0][rand_col], decimal_places),
            round(nc_mat[0][rand_col], decimal_places))

class TestShape(TestCase):
    def test_shape(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

    def test_shape_big(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2000, 2000, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

    def test_shape_1x1(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

    def test_shape_long(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1, 1000, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

    def test_shape_tall(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(1000, 1, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

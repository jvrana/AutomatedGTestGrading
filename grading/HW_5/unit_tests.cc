//
// Created by Justin Vrana on 2019-02-04.
//

#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include <stdlib.h>
#include "gtest/gtest.h"
#include "utilities.h"
#include "typed_matrix.h"
#include <fstream>
#include "gtestnodeath.h"
#include <vector>



using std::string;
using std::vector;

#define EPSILON DBL_EPSILON*10.0 // double tolerance
#define DBL_PRECISION 0.0001
#define Q1POINTS 100.0
#define Q2POINTS 100.0
#define Q3POINTS 100.0
#define Q4POINTS 100.0
#define Q5POINTS 100.0
#define NUM_QUESTIONS 5 // overestimated number of questions
#define GTEST_COUT_GRADE std::cerr       << "[    GRADE ] "

/*
 * This is the base test class for all of the methods.
 *
 * All other tests inherit from this base class. It
 * contains several methods for building vector matrices,
 * random matrices, etc. for testing purposes.
 *
 * You can also see some file creation stuff that is relevant for
 * the homework.
 */
class BaseTest : public ::testing::Test {
protected:

    /*!
     * Compare two doubles, with relaxed tolerances
     * @param a
     * @param b
     * @return
     */
    bool compare_dbl(double a, double b) {
        if (fabs(a - b) < EPSILON) {
            return true;
        }
        return false;
    }

    /*!
     * Creates a random double.
     *
     * @param min
     * @param max
     * @return
     */
    double random_dbl(double min, double max) {
        double range = (max - min);
        double div = RAND_MAX / range;
        return min + (rand() / div);
    }

    /*!
     * Random integer between min and max
     * @param min
     * @param max
     * @return
     */
    int random_int(int min, int max) {
        if (max == 0) {
            return 0;
        }
        return rand() % max + min;
    }

    /*!
     * Create an integer matrix
     * @param r
     * @param c
     * @param min
     * @param max
     * @return
     */
    TypedMatrix<int> int_typed_matrix(int r, int c, int min, int max) {
        vector<vector<int>> x = int_matrix(r, c, min, max);
        return int_typed_matrix(x);
    }

    /*!
     * Create an integer matrix
     * @param v
     * @return
     */
    TypedMatrix<int> int_typed_matrix(const vector<vector<int>> &v) {
        int rows = v.size(),
                cols;
        if (rows > 0) {
            cols = v[0].size();
        }
        TypedMatrix<int> m = safe_int_construct(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                m.set(i, j, v[i][j]);
            }
        }
        return m;
    }

    /*!
     * Create a random TypedMatrix of size rxc
     * @param size
     * @return
     */
    TypedMatrix<double> dbl_typed_matrix(int r, int c, double min, double max) {
        vector<vector<double>> x = dbl_matrix(r, c, min, max);
        return dbl_typed_matrix(x);
    }

    /*!
     * Create TypedMatrix from double matrix
     * @param v
     * @return
     */
    TypedMatrix<double> dbl_typed_matrix(const vector<vector<double>> &v) {
        int rows = v.size(),
            cols;
        if (rows > 0) {
            cols = v[0].size();
        }
        TypedMatrix<double> m = safe_dbl_construct(rows, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                m.set(i, j, v[i][j]);
            }
        }
        return m;
    }

    /*!
     * Determines the convention the student was using to construct,
     * get, and set matrices. Normally, it should be TypedMatrix(row, col),
     * but somehow the previous homework specified TypedMatrix(col, row).
     *
     * determines how the student interpreted the instructions.
     *
     * In the very least, .set and .get should be consistent with each other by the
     * constructor and .set & .get methods may be reversed in respect to each other.
     *
     * 1 means normal (row, col) convention, -1 means reversed (col, row convention), 0
     * means convention was unable to be determined.
     */
    int convention() {
        TypedMatrix<double> m = TypedMatrix<double>(2, 10);
        try {
            m.set(1, 9, 1.0);
            double x = m.get(1, 9);
            if (x == 1.0) {
                return 1;
            }
        } catch (const std::exception& e) {
            m.set(9, 1, 1.0);
            double x = m.get(9, 1);
            if (x == 1.0) {
                return -1;
            }
        }
        return 0;
    }

    /*!
     * Creates a random double vector.
     *
     * @param size number of doubles in the vector
     * @param min minimum double
     * @param max maximum double
     * @return vector of doubles
     */
     vector<int> int_vector(int size, int min, int max) {
         vector<int> v;
         v.resize(size);
         for (int i = 0; i < size; i++) {
             v[i] = random_int(min, max);
         }
         return v;
     }

    /*!
     * Creates a random int vector.
     *
     * @param size number of doubles in the vector
     * @param min minimum double
     * @param max maximum double
     * @return vector of doubles
     */
    vector<double> dbl_vector(int size, double min, double max) {
        vector<double> v;
        v.resize(size);
        for (int i = 0; i < size; i++) {
            v[i] = random_dbl(min, max);
        }
        return v;
    }

     /*!
    * Create a random vector<vector<double>> matrix of doubles.
    *
    * @param r
    * @param c
    * @param mn
    * @param mx
    * @returnFc
    */
    vector<vector<double>> dbl_matrix(int r, int c, double mn, double mx) {
        vector<vector<double>> x;
        x.resize(r);
        for (int i = 0; i < r; i++) {
            x[i] = dbl_vector(c, mn, mx);
        }
        return x;
    }

     /*!
    * Create a random vector<vector<int>> matrix of ints.
    *
    * @param r
    * @param c
    * @param mn
    * @param mx
    * @return
    */
    vector<vector<int>> int_matrix(int r, int c, int mn, int mx) {
        vector<vector<int>> x;
        x.resize(r);
        for (int i = 0; i < r; i++) {
            x[i] = int_vector(c, mn, mx);
        }
        return x;
    }

     /*!
      * Print double vector contents
      */
      void print_vector(vector<double> &v) {
          std::cout << "[ ";
          vector<double>::iterator i;
          for (i = v.begin(); i != v.end(); i++) {
              std::cout << (*i) << " ";
          }
          std::cout << "]" << std::endl;
      }

      /*!
      * Print double vector contents
      */
      void print_vector(vector<int> &v) {
          std::cout << "[ ";
          vector<int>::iterator i;
          for (i = v.begin(); i != v.end(); i++) {
              std::cout << (*i) << " ";
          }
          std::cout << "]" << std::endl;
      }

      /*!
      * Print double vector contents
      */
      void print_vector(vector<string> &v) {
          std::cout << "[ ";
          vector<string>::iterator i;
          for (i = v.begin(); i != v.end(); i++) {
              std::cout << (*i) << " ";
          }
          std::cout << "]" << std::endl;
      }

      /*!
       * Save csv from matrix of strings to a specified path
       *
       * @param v
       * @param path
       * @return
       */
      string save_csv(vector<vector<string>> &v, const string& path) {
          std::ofstream outfile;
          outfile.open(path);
          std::cout << "Saving file" << std::endl;
          int rows = v.size();

          for (int i = 0; i < rows; i++) {
              for (int j = 0; j < v[i].size(); j++) {
                  outfile << v[i][j];
                  if (j < v[i].size()-1) {
                      outfile << ",";
                  }
              }
              if (i < rows-1) {
                  outfile << "\n";
              }
          }
          outfile.close();
          return path;
      }

      /*!
       * Convert matrix of doubles to matrix of strings
       *
       * @param v
       * @return
       */
      vector<vector<string>> to_vector_string(const vector<vector<double>> &v) {
          vector<vector<string>> s;
          s.resize(v.size());
          auto to_s = [](vector<double> x) {
              vector<string> s;
              s.resize(x.size());
              std::transform(
                      x.begin(),
                      x.end(),
                      s.begin(),
                      static_cast<std::string(*)(double)>(std::to_string)
                      );
              return s;
          };
          std::transform(v.begin(), v.end(), s.begin(), to_s);
          return s;
      }

      /*!
       * Save csv from matrix of strings
       *
       * @param v
       * @return
       */
      string save_csv(vector<vector<string>> &v) {
          string default_path = "tmp.csv";
          return save_csv(v, default_path);
      }

      /*!
       * Save csv from matrix of doubles
       *
       * @param v
       * @return
       */
      string save_csv(vector<vector<double>> &v) {
          vector<vector<string>> s = to_vector_string(v);
          return save_csv(s);
      }

    /*!
     * Create a random double csv of with "r" rows and "c" columns. With doubles
     * inclusively between "mn" and "mx"
     *
     * @param r num rows
     * @param c num cols
     * @param mn min double
     * @param mx max double
     * @return
     */
    string random_csv(int r, int c, int mn, int mx) {
          vector<vector<double>> x = dbl_matrix(r, c, mn, mx);
          return save_csv(x);
    }

    /*!
     * Safely construct a double TypedMatrix depending on the student's r vs c constructor convention
     * @param r rows
     * @param c columns
     * @return
     */
    TypedMatrix<double> safe_dbl_construct(int r, int c) {
        int con = convention();
        if (con == 1) {
            return TypedMatrix<double>(r, c);
        } else if (con != 1) {
            return TypedMatrix<double>(c, r);
        } else {
            throw std::range_error("Matrix dimension doesn't match.");
        }
    }

    /*!
     * Safely construct a double TypedMatrix depending on the student's r vs c constructor convention
     * @param r
     * @param c
     * @param min
     * @param max
     * @return
     */
    TypedMatrix<double> safe_dbl_construct(int r, int c, double min, double max) {
        int con = convention();
        if (con == 1) {
            return TypedMatrix<double>(r, c);
        } else if (con != 1) {
            return TypedMatrix<double>(c, r);
        } else {
            throw std::range_error("Matrix dimension doesn't match.");
        }
    }

    /*!
     * Safely construct a int TypedMatrix depending on the student's r vs c constructor convention
     * @param r rows
     * @param c columns
     * @return
     */
    TypedMatrix<int> safe_int_construct(int r, int c) {
        int con = convention();
        if (con == 1) {
            return TypedMatrix<int>(r, c);
        } else if (con != 1) {
            return TypedMatrix<int>(c, r);
        } else {
            throw std::range_error("Matrix dimension doesn't match.");
        }
    }

};

/*
 * Define the base test classes for each of the questions.
 *
 * Each question should inherit from the main Question base,
 * which holds three static vectors (number of tests, number of
 * passing tests, total point value). Each inherited class
 * should get its own unique integer id. To get the number of tests,
 * number of passing tests, or total point values for a question,
 * just access the static vectors with the corresponding unique
 * int id.
 *
 * The individual weights for each question can be adjusted via
 * the "totals" static vector. The number of tests and number
 * of passing tests are computed automatically.
 *
 * To get the total grade, call "grade()". To get a grade break down
 * of each question, call "question_grades()".
 */

class Question : public BaseTest {
public:
    static vector<int> num_tests;
    static vector<int> num_passed;
    static vector<double> totals;

protected:
    int total_points;
    int id = -1;

    Question() {
        num_tests.resize(NUM_QUESTIONS);
        num_passed.resize(NUM_QUESTIONS);
        totals.resize(NUM_QUESTIONS);
    }

    virtual vector<double> question_grades() {
        vector<double> grade;
        grade.resize(NUM_QUESTIONS);
        for (int i = 0; i < NUM_QUESTIONS; i++) {
            if (totals[i] > 0) {
                grade[i] = (double) num_passed[i]/num_tests[i] * totals[i];
            }
        }
        return grade;
    }

    virtual double grade() {

        double t, v;
        for (int i = 0; i < NUM_QUESTIONS; i++) {
            if (totals[i] > 0) {
                v += (double) num_passed[i]/num_tests[i] * totals[i];
                t += totals[i];
            }
        }
        return v/t;
    }

    void print_grade() {
        vector<double> q = question_grades();
        GTEST_COUT_GRADE << grade() * 100.0 << "%" << std::endl;
        GTEST_COUT_GRADE << "Question breakdown: " << std::endl;
        std::cout << grade() * 100.0 << std::endl;
        std::cout << "Question breakdown: " << std::endl;
        print_vector(q);
    }

    virtual void TearDown() {
        if (!HasFailure()) {
            num_passed[id]++;
        }
        print_grade();
    }
};

vector<int> Question::num_tests;
vector<int> Question::num_passed;
vector<double> Question::totals;
// get number of questions

class Question1 : public Question {
protected:
    Question1() {
        id = 0;
        totals[id] = Q1POINTS;
        num_tests[id]++;
    }
};

class Question2 : public Question {
protected:
    Question2() {
        id = 1;
        totals[id] = Q2POINTS;
        num_tests[id]++;
    }
};

class Question3 : public Question {
protected:
    Question3() {
        id = 2;
        totals[id] = Q3POINTS;
        num_tests[id]++;
    }
};

class Question4 : public Question {
protected:
    Question4() {
        id = 3;
        totals[id] = Q4POINTS;
        num_tests[id]++;
    }
};

class Question5 : public Question {
protected:
    Question5() {
        id = 4;
        totals[id] = Q5POINTS;
        num_tests[id]++;
    }
};


/*
 * Question 1: sort_by_magnitude *************************************************
 * Write a function called sort_by_magnitude that takes a reference to a vector of
 * doubles and sorts it with a comparison function that says that x < y if and only
 * if the absolute value of x is less than the absoilute value of y. For example,
 * -5 would be great than 4 because |-5| > |4|. Use the generic algorithm sort
 * (which you can look up on the c++ reference we page) and a lambda expression
 * for your comparison function. Declare method in utilities.h as well,
 * and implement it in utilities.cc.
 */

class SortTests : public Question1,
                    public ::testing::WithParamInterface<int> {
};

/*
 * Parameterized test for testing if the sort_by_magnitude method
 * is correctly sorting the vector.
 *
 * We didn't really specify whether was supposed to be sorted
 * in place or not, so test covers both situations.
 */
TEST_P(SortTests, SortByMag) {
    int size = GetParam();
    double  min = -1000,
            max = 1000.0;

    vector<double> x = dbl_vector(size, -100.0, 100.0);
    vector<double> original_vector = x;
    vector<double> sorted = sort_by_magnitude(x);

    ASSERT_EQ(sorted.size(), x.size()) << "Sorted vector and original vector should be same size";

    // check if elements are sorted
    if (x.size() > 1) {
        double prev, val;
        prev = sorted[0];
        for (int i = 1; i < x.size(); i++) {
            val = sorted[i];
            EXPECT_GE(std::fabs(val), std::fabs(prev)) << "Elements are unsorted";
            prev = val;
        }
    }
};

/*
 * parameterizes the SortTests, TEST_P class above.
 */
INSTANTIATE_TEST_CASE_P(SortTests,
        SortTests,
        ::testing::Range(0, 1000, 100) // size of the first array
);

/*
 * Question 2 *************************************************
 * Rewrite the TypedMatrix class with vectors instead of TypedArrays.
 * The interface to the user should be identical to what we specified
 * in that previous homework.
 */

void CheckNoDeathWithDeath(const TypedMatrix<double> & m, int r, int c) {
    if (r > 0 and c > 0) {
        ASSERT_NO_DEATH({
                            m.get(0, 0);
                            m.get(r-1, c-1);
                        }, ".*");
    }
}

void CheckNoDeathWithDeath(const TypedMatrix<int> & m, int r, int c) {
    if (r > 0 and c > 0) {
        ASSERT_NO_DEATH({
                            m.get(0, 0);
                            m.get(r-1, c-1);
                        }, ".*");
    }
}

void CheckOutOfBounds(const TypedMatrix<double> & m, int r, int c) {
    ASSERT_NO_DEATH({m.get(r, c);}, ".*");
    EXPECT_ANY_THROW(m.get(r,c-1));
    EXPECT_ANY_THROW(m.get(r-1,c));
}

void CheckOutOfBounds(const TypedMatrix<int> & m, int r, int c) {
    ASSERT_NO_DEATH({m.get(r, c);}, ".*");
    EXPECT_ANY_THROW(m.get(r,c-1));
    EXPECT_ANY_THROW(m.get(r-1,c));
}

class MatrixTests : public Question2,
                  public ::testing::WithParamInterface<std::tuple<int, int, int>> {
};

TEST_P(MatrixTests, TypedMatrixEmptyConstructor) {
    std::tuple<int, int, bool> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    int check_values = std::get<2>(params);
    ASSERT_NO_DEATH({
        safe_dbl_construct(r, c);
    }, ".*");

    TypedMatrix<double> m = safe_dbl_construct(r, c);
    CheckNoDeathWithDeath(m, r, c);

    if (check_values == 1) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ASSERT_DOUBLE_EQ(m.get(i, j), double());
            }
        }
    } else if (check_values == 2) {
        CheckOutOfBounds(m, r, c);
    }
};

TEST_P(MatrixTests, TypedMatdbl_typed_matrixrixRandomDoubleConstructor) {
    std::tuple<int, int, bool> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    int check_values = std::get<2>(params);
    ASSERT_NO_DEATH(
            dbl_typed_matrix(r, c, -10000.0, 10000.0), ".*"
    );

    vector<vector<double>> x = dbl_matrix(r, c, -10000.0, 10000.0);
    TypedMatrix<double> m = dbl_typed_matrix(x);
    CheckNoDeathWithDeath(m, r, c);

    if (check_values == 1) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ASSERT_DOUBLE_EQ(m.get(i, j), x[i][j]);
            }
        }
    } else if (check_values == 2) {
        CheckOutOfBounds(m, r, c);
    }
};

TEST_P(MatrixTests, TypedMatrixRandomIntConstructor) {
    std::tuple<int, int, bool> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);
    int check_values = std::get<2>(params);

    ASSERT_NO_DEATH(
            int_typed_matrix(r, c, -10000, 10000), ".*"
    );

    // check in bounds
    vector<vector<int>> x = int_matrix(r, c, -100, 100);
    TypedMatrix<int> m = int_typed_matrix(x);
    CheckNoDeathWithDeath(m, r, c);

    if (check_values == 1) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ASSERT_EQ(m.get(i, j), x[i][j]);
            }
        }
    } else if (check_values == 2) {
        CheckOutOfBounds(m, r, c);
    }
};

class MatrixOperatorTests : public Question2,
                    public ::testing::WithParamInterface<std::tuple<int, int>> {
};

TEST_P(MatrixOperatorTests, Copy) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    ASSERT_NO_DEATH({
        TypedMatrix<double> m = dbl_typed_matrix(r, c, -100, 100);
        TypedMatrix<double> a;
        a = m;
    }, ".*");
    TypedMatrix<double> m = dbl_typed_matrix(r, c, -100, 100);
    TypedMatrix<double> a;
    a = m;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            ASSERT_EQ(m.get(i, j), a.get(i, j));
        }
    }
};

TEST_P(MatrixOperatorTests, Add) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    ASSERT_NO_DEATH({
        TypedMatrix<double> m1 = dbl_typed_matrix(r, c, -100, 100);
        TypedMatrix<double> m2 = dbl_typed_matrix(r, c, -100, 100);
        m1 + m1;
    }, ".*");

    vector<vector<double>> x1 = dbl_matrix(r, c, -100, 100);
    vector<vector<double>> x2 = dbl_matrix(r, c, -100, 100);

    TypedMatrix<double> m1 = dbl_typed_matrix(x1);
    TypedMatrix<double> m2 = dbl_typed_matrix(x2);
    TypedMatrix<double> m3 = m1 + m2;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            double expected = x1[i][j] + x2[i][j];
            double v = m3.get(i, j);
            ASSERT_NEAR(v, expected, DBL_PRECISION);
        }
    }
};

TEST_P(MatrixOperatorTests, Equal) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    ASSERT_NO_DEATH({
        TypedMatrix<double> m1 = dbl_typed_matrix(r, c, -100, 100);
        TypedMatrix<double> m2 = dbl_typed_matrix(r, c, -100, 100);
        m1 == m1;
    }, ".*");

    vector<vector<double>> x1 = dbl_matrix(r, c, -100, 100);
    vector<vector<double>> x2 = dbl_matrix(r, c, -100, 100);

    TypedMatrix<double> m1 = dbl_typed_matrix(x1);
    TypedMatrix<double> m2 = dbl_typed_matrix(x1);
    TypedMatrix<double> m3 = dbl_typed_matrix(x2);

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
    ASSERT_FALSE(m2 == m3);
};



TEST_P(MatrixOperatorTests, MatrixMult) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);
    ASSERT_NO_DEATH({
        TypedMatrix<double> m1 = dbl_typed_matrix(r, 5, -100, 100);
        TypedMatrix<double> m2 = dbl_typed_matrix(5, c, -100, 100);
        m1 * m2;
        }, ".*");

    int common = 5;
    vector<vector<double>> x1 = dbl_matrix(r, common, -100, 100);
    vector<vector<double>> x2 = dbl_matrix(common, c, -100, 100);
    TypedMatrix<double> m1 = dbl_typed_matrix(x1);
    TypedMatrix<double> m2 = dbl_typed_matrix(x2);
    TypedMatrix<double> m3 = m1 * m2;

    vector<vector<double>> expected = dbl_matrix(r, c, 0, 1);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            double tmp;
            for (int k = 0; k < common; k++) {
                tmp += x1[i][k] * x2[k][j];
            }
            expected[i][j] = tmp;
        }
    }


}

TEST_P(MatrixOperatorTests, MultAssign) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
        c = std::get<1>(params);
    ASSERT_NO_DEATH({
        TypedMatrix<double> m1 = dbl_typed_matrix(r, c, -100, 100);
        TypedMatrix<double> m2 = dbl_typed_matrix(r, c, -100, 100);
        m1 *= m2;
    }, ".*");

    vector<vector<double>> x1 = dbl_matrix(r, c, -100, 100);
    vector<vector<double>> x2 = dbl_matrix(r, c, -100, 100);

    TypedMatrix<double> m1 = dbl_typed_matrix(x1);
    TypedMatrix<double> m2 = dbl_typed_matrix(x2);
    m1 *= m2;


    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            double expected = x1[i][j] * x2[i][j];
            double v = m1.get(i, j);
            ASSERT_NEAR(v, expected, DBL_PRECISION);
        }
    }
};

TEST_P(MatrixOperatorTests, AddAssign) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);
    ASSERT_NO_DEATH({
                        TypedMatrix<double> m1 = dbl_typed_matrix(r, c, -100, 100);
                        TypedMatrix<double> m2 = dbl_typed_matrix(r, c, -100, 100);
                        m1 += m2;
                    }, ".*");

    vector<vector<double>> x1 = dbl_matrix(r, c, -100, 100);
    vector<vector<double>> x2 = dbl_matrix(r, c, -100, 100);

    TypedMatrix<double> m1 = dbl_typed_matrix(x1);
    TypedMatrix<double> m2 = dbl_typed_matrix(x2);
    m1 += m2;


    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            double expected = x1[i][j] + x2[i][j];
            double v = m1.get(i, j);
            ASSERT_NEAR(v, expected, DBL_PRECISION);
        }
    }
};



INSTANTIATE_TEST_CASE_P(MatrixTests,
        MatrixTests,
        ::testing::Combine(
            testing::Values(0, 1, 2, 10),
            testing::Values(1, 2, 10),
            testing::Values(0, 1, 2)
        )
);

INSTANTIATE_TEST_CASE_P(MatrixOperatorTests,
        MatrixOperatorTests,
        ::testing::Combine(
            testing::Values(1, 1, 2, 10),
            testing::Values(1, 2, 10)
        )
);

/*
 * Question 3 *************************************************
 * Write a method in in utilities.h and utilities.cc
 *
 * TypedMatrix<double> read_matrix_csv(const string path);
 *
 * that reads a comma separated value (CSV) file of doubles into
 * a matrix and returns it. If there are any errors in the format
 * of the CVS file, or missing values (so the CSV does not represent
 * a matrix), throw an exception. Spaces and tabs should be ok in
 * between commas, but newlines should only be used to terminate
 * a row of the matrix. Place method in utilities.h and utilities.cc
 */

class ReadTests : public Question3,
                    public ::testing::WithParamInterface<std::tuple<int, int>> {
};

TEST_P(ReadTests, ReadRandomCSV) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
    c = std::get<1>(params);

    vector<vector<double>> x = dbl_matrix(r, c, -1000.0, 1000.0);
    string path = save_csv(x);

    ASSERT_NO_DEATH(read_matrix_csv(path), ".*");

    TypedMatrix<double> m = read_matrix_csv(path);

    int rows = x.size(),
        cols;
    if (rows > 0) {
        CheckNoDeathWithDeath(m, rows, cols);
    }
    for (int i = 0; i < x.size(); i++) {
        cols = x[0].size();
        for (int j = 0; j < cols; j++) {
            ASSERT_NEAR(m.get(i, j), x[i][j], DBL_PRECISION);
        }
    }
}

/*
 * Add extra double on one of the rows, which should compromise
 * the abilitiy to read the csv file.
 */
TEST_P(ReadTests, ReadRandomBrokenCSV) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);

    GTEST_COUT << "Rows: " << r << " Cols: " << c << std::endl;
    vector<vector<double>> x = dbl_matrix(r, c, -1000.0, 1000.0);
    vector<vector<string>> s = to_vector_string(x);

    if (r > 0) {
        int i = random_int(0, r-1);
        vector<string>& row = s[i];
        row.push_back("1.0");
        string path = save_csv(s, "tmp.csv");

        ASSERT_NO_DEATH(read_matrix_csv(path), ".*"); // should not crash, but throw error
        if (r > 1) {
            ASSERT_ANY_THROW(read_matrix_csv(path)); // should throw some kind of error
        } else {
            read_matrix_csv(path); // should not throw an error
        }
    }
}


class ReadTestsWhiteSpace : public Question3,
                  public ::testing::WithParamInterface<std::tuple<int, int, char>> {
};

TEST_P(ReadTestsWhiteSpace, ReadRandomCSVWithSpaces) {
    std::tuple<int, int, char> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);
    char whitespace = std::get<2>(params);
    GTEST_COUT << "Rows: " << r << " Cols: " << c << std::endl;
    vector<vector<double>> x = dbl_matrix(r, c, -1000.0, 1000.0);
    vector<vector<string>> s = to_vector_string(x);
    string fpad, bpad;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            fpad.assign(random_int(1, 3), whitespace); // add between 1 and 3 whitespace characters
            bpad.assign(random_int(1, 3), whitespace);
            s[i][j] = fpad + s[i][j] + bpad;
        }
    }

    string path = save_csv(s, "tmp.csv");
}

INSTANTIATE_TEST_CASE_P(ReadTests,
        ReadTests,
        ::testing::Combine(
            testing::Values(1, 10, 100), // num rows
            testing::Values(1, 10, 100)     // num cols
        )
);

INSTANTIATE_TEST_CASE_P(ReadTestsWhiteSpace, ReadTestsWhiteSpace,
        ::testing::Combine(
                testing::Values(1, 10, 100), // num rows
                testing::Values(1, 10, 100), // num cols
                testing::Values(' ', '\t')  // white space character
                ));

/*
 * Question 4 *************************************************
 * Write a method
 *
 * void write_matrix_csv(const TypedMatrix<double> &matrix, const string path);
 *
 * that writes a comma separated value (CSV) file of doubles from a matrix.
 * Test that you can write and read a matrix and get the same matrix back.
 * Place method in utilities.h and utilities.cc
 */

class WriteTests : public Question4,
                  public ::testing::WithParamInterface<std::tuple<int, int>> {
};

TEST_P(WriteTests, WriteRandomCSV) {
    std::tuple<int, int> params = GetParam();
    int r = std::get<0>(params),
            c = std::get<1>(params);

    vector<vector<double>> x = dbl_matrix(r, c, -1000.0, 1000.0);
    ASSERT_NO_DEATH(dbl_typed_matrix(x), ".*");
    TypedMatrix<double> m = dbl_typed_matrix(x);

    ASSERT_NO_DEATH(write_matrix_csv(m, "tmp.csv"), ".*");

    write_matrix_csv(m, "tmp.csv");
    }

INSTANTIATE_TEST_CASE_P(WriteTests,
        WriteTests,
        ::testing::Combine(
        testing::Values(0, 1, 10, 100),
        testing::Values(1, 10, 100)
)
);

/*
 * Question 5 *************************************************
 * Write a method
 *
 * map<string, int> occurrence_map(const string path);
 *
 * that reads in an ascii text file and returns an assocation where each
 * key is a word in the text file and each value is the number of occurrences
 * of that word. Words consist entirely of either alpha-numeric characters
 * (a through z, A through Z, 0 through 9) or the single quote characters '.
 * constitutes only 26*2 + 10 + 1 = 63 valid characters. Valid words
 * include dont' 10xgenomics bob, etc. Invalid words are $$f(x)$$ Sh%6fh,
 * not_a_word_because_of_underscores. The method should be case-insensitive
 * and should store the keys as lowercase. Ignore punctuation so that I'm done!
 * results in keys i'm and done. Quotes should be handles as well so that I'm
 * so "done" results in keys i'm, so, and done. Consider the following examples:
 */

#define MAPDELIM " %%:%% "

class BaseMapTest : public Question5 {
public:

    static std::map<string, int> load_mapping(const string &path) {
        std::cout << "loading " << path << std::endl;

        std::ifstream infile;
        infile.open(path, std::ifstream::in);
        string line, token;
        vector <string> temp_tokens;
        temp_tokens.resize(2);

        std::map<string, int> m;
        while (std::getline(infile, line)) {
            int n = line.find(MAPDELIM);
            if (n != std::string::npos) {
                string key = line.substr(0, n);
                string delim = MAPDELIM;
                int num = std::stoi(line.substr(n + delim.size()));
                m[key] = num;
            }
        }
        infile.close();
        std::cout << m.size() << "!\n";

        return m;
    }

};

class MapKeywordTests : public Question5,
                  public ::testing::WithParamInterface< std::pair<const string, int> > {
};

string expected_path_ = "AnswerMap.txt";
string txt_path_ = "lorem_ipsum_explain.txt";
std::map<string, int> expected_map_ = BaseMapTest::load_mapping(expected_path_);

/*!
 * Subroutine to check if occurrence_map causes death
 */
void CheckOccurrenceDeath() {
    ASSERT_NO_DEATH(occurrence_map(txt_path_), ".*");
}

TEST_F(BaseMapTest, CheckNoExtraKeywords) {
    CheckOccurrenceDeath();
}

TEST_P(MapKeywordTests, CheckForKeywords) {
    CheckOccurrenceDeath();
    std::map<string, int> map = occurrence_map(txt_path_);

    std::pair<const string, int> pair = GetParam();

    string key = std::get<0>(pair);

    ASSERT_GT(map[key], 0);
}

TEST_P(MapKeywordTests, CheckNumInstanceCorrect) {
    CheckOccurrenceDeath();
    std::map<string, int> map = occurrence_map(txt_path_);

    std::pair<const string, int> pair = GetParam();

    string key = std::get<0>(pair);
    int n = std::get<1>(pair);

    ASSERT_EQ(map[key], n);
}

INSTANTIATE_TEST_CASE_P(MapKeywordTests, MapKeywordTests,
        ::testing::ValuesIn(expected_map_) // size of the first array
);
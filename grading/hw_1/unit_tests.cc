//
// Created by Rajendra Hathwar on 2020-01-18.
//

#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include <stdlib.h>
#include "gtest/gtest.h"
#include "fraction.h"
#include "complex.h"
#include <fstream>
#include "gtestnodeath.h"
#include <vector>



using std::string;
using std::vector;

#define EPSILON DBL_EPSILON*10.0 // double tolerance
#define DBL_PRECISION 0.0001
#define Q1POINTS 100.0
#define Q2POINTS 100.0
#define NUM_QUESTIONS 2 // overestimated number of questions
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


/*
 * Question 1: reduce *************************************************
 */

class FractionTests : public Question1,
                    public ::testing::WithParamInterface<std::tuple<int, int>> {
};

/*
 * Parameterized test for testing if the reduce method
 * is correctly reducing the fraction
 */
TEST_P(FractionTests, Reduce) {
    std::tuple<int, int> params = GetParam();
    Fraction a = (Fraction) {std::get<0>(params), std::get<1>(params)};

    // Call the student's api
    Fraction result = reduce(a);
    Fraction expected;

    if (a.num == 0 && a.den == 0) {
        ASSERT_EQ(result.num, 0);
        ASSERT_EQ(result.den, 0);
        return;
    }

    if (a.num == a.den ) {
        ASSERT_EQ(result.num, 1);
        ASSERT_EQ(result.den, 1);
        return;
    }

    if (a.num == 0 ) {
        ASSERT_EQ(result.num, 0);
        ASSERT_EQ(result.den, 1);
        return;
    }

    if (a.den == 0) {
        ASSERT_EQ(result.num, 1);
        ASSERT_EQ(result.den, 0);
        return;
    }

    int sign = 1;
    if (a.den < 0){ 
        sign = -1;
    }

    int gcf, i = 1;
    while (i <= abs(a.num) && i <= abs(a.den)){
        if (a.num % i == 0 && a.den % i == 0){ 
            gcf = i;
        }
        i++;
    }

    expected = (Fraction) {sign * a.num / gcf, sign * a.den / gcf};
    ASSERT_EQ(result.num, expected.num);
    ASSERT_EQ(result.den, expected.den);
};

/*
 * parameterizes the FractionTests, TEST_P class above.
 */
INSTANTIATE_TEST_CASE_P(FractionTests,
        FractionTests,
        testing::Combine(
            testing::Range(-10, 10, 1),
            testing::Range(-1000, 1000, 100))
);

/*
 * Question 2 *************************************************
 */

class ComplexTests : public Question2,
                  public ::testing::WithParamInterface<std::tuple<double, double, double, double>> {
};

TEST_P(ComplexTests, Add) {
    std::tuple<double, double, double, double> params = GetParam();
    Complex a = (Complex) {std::get<0>(params), std::get<1>(params)},
            b = (Complex) {std::get<2>(params), std::get<3>(params)};

    // Call the student's api
    Complex result = add(a, b);
    Complex expected = (Complex) {a.real + b.real, a.im + b.im};

    ASSERT_NEAR(result.real, expected.real, DBL_PRECISION);
    ASSERT_NEAR(result.im, expected.im, DBL_PRECISION);
};

TEST_P(ComplexTests, Negate) {
    std::tuple<double, double, double, double> params = GetParam();
    Complex a = (Complex) {std::get<0>(params), std::get<1>(params)},
            b = (Complex) {std::get<2>(params), std::get<3>(params)};

    // Call the student's api
    Complex result = negate(a);
    Complex expected = (Complex) {-1.0 * a.real, -1.0 * a.im};

    ASSERT_NEAR(result.real, expected.real, DBL_PRECISION);
    ASSERT_NEAR(result.im, expected.im, DBL_PRECISION);
};

TEST_P(ComplexTests, Multiply) {
    std::tuple<double, double, double, double> params = GetParam();
    Complex a = (Complex) {std::get<0>(params), std::get<1>(params)},
            b = (Complex) {std::get<2>(params), std::get<3>(params)};

    // Call the student's api
    Complex result = multiply(a, b);
    Complex expected = (Complex) {(a.real * b.real) - (a.im * b.im), (a.real * b.im) + (a.im * b.real)};

    ASSERT_NEAR(result.real, expected.real, DBL_PRECISION);
    ASSERT_NEAR(result.im, expected.im, DBL_PRECISION);
};

TEST_P(ComplexTests, Magnitude) {
    std::tuple<double, double, double, double> params = GetParam();
    Complex a = (Complex) {std::get<0>(params), std::get<1>(params)},
            b = (Complex) {std::get<2>(params), std::get<3>(params)};

    // Call the student's api
    double result = magnitude(a);
    double expected = (double) sqrt(a.real*a.real + a.im*a.im);

    ASSERT_NEAR(result, expected, DBL_PRECISION);
};

INSTANTIATE_TEST_CASE_P(ComplexTests,
        ComplexTests,
        testing::Combine(
            testing::Range(-1.0, 2.0, 1),
            testing::Range(0.0, 3.0, 1),
            testing::Range(1.0, 4.0, 1),
            testing::Range(2.0, 5.0, 1))
);

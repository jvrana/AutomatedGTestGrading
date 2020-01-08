# ECEP520/ECE590 Grading script

These are a collection of grading scripts to grade c/c++ homeworks for ECEP520/ECE590
students. 

Briefly, these scripts do the following:

`pull.sh`
1. Clones student's homework code from student Github repos into `tmp/<login>/<HW>` folder.
2. If repo has been cloned, pull latest code from repo.


`grade.sh`
1. Opens a new Docker container containing c/c++ dependencies
2. Copies student's code into container
3. Copies code from `grading/<HW>/*` into container
4. Runs unit test file `grading/<HW>/unit_tests.cc`
5. Summarized results and errors into `results/<HW>/<login>.out`
6. Summarizes all students' results in `results/<HW>/summary.csv`

## Usage

**IMPORTANT**: Remove all spaces from the input csv. Many students have more than
just a first and last name, and since BASH is not great at parsing csvs (using IFS="," for example
will separate commas AND spaces) simply avoid trouble by removing any spaces from the file.

### Setup

1. install docker
2. install git and credentials

### Pulling Student Repositories

Students are expected to have shared repositories with the TAs and Professors.
The location of these should be `www.github.com/<login>/<class_name>`

Within this repository, homeworks should be listed in the top level as in:

```
HW_1
HW_2
HW_3
etc.
```

To pull student repositories, edit the `students.csv` for all of the students.
Then run the following script:

```bash
sh pull.sh -i students.csv
```

This will pull each of the student's repos to the `tmp/<login>` folder.

Re-run this script each week before grading.

**NOTE**: `pull.sh` contains code to checkout commits from before the homework's
due date. Take a look at the DUE_DATE argument.

### Making the `unit_tests.cc` file

The `unit_tests.cc` file contains GTests for grading a student's homework.
Using Google's GTest test suite can be a complicated subject, so it is
recommended review the GTest documentation.

For grading student's code, the TA/Professor writes of a suite of tests
in the `grading/<HW>/unit_tests.cc` file using GTest. Each successful test
increases the students point total. These tests are grouped into `Question`
test classes, from which other test classes or test methods are inherited. 
Each `Question` class gets its own point total, so that individual tests
or questions can be properly weighted. For example, the following
tests below defines `Question1` as being worth 100 points:

```c++
#define Q1POINTS 100.0

class Question1 : public Question {
protected:
    Question1() {
        id = 0;
        totals[id] = Q1POINTS;
        num_tests[id]++;
    }
};
```

Tests can be derived from this class, as in the following example 
from `grading/HW_5/unit_tests.cc` which runs many parameterized tests 
for Question1:

```c++
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
```

Please see the example located in `grading/HW_5`. Especially take a look
at `grading/HW_5/unit_tests.cc`. 

#### Gotchas

Students come up with all kinds of clever ways to introduce errors into their 
code. The most common being **segmentation faults** for which it
is (as far as I know) almost impossible to test. Most **segmentation faults** will
terminate these automated grading scripts. The `grading/HW_5/gtestnodeath.h` contains
code to rescues a few situations in which segmentation faults can occur, instead
causing a test failure rather than program termination. This code is automatically
incorporated in these scripts. However, it does not catch all cases of seg faults,
and so the TA will have to use deft judgment to correct the student's code
and assign and appropriate grade.

### Running the Automated Grading Script

To run the grading script on all students, run

```bash
grade.sh -h <HW_X> -i students.csv
```
Replacing `<HW_X>` with the name of the expected homework directory (e.g. `HW_1`)

To grade a single student (e.g. login "mjane" and homework "HW_1"), run

```$xslt
sh grade.sh -h <HW_X> -l mjane
```

Results of grading can be found in the `results` folder. 
A summary
of the grading result can also be found in `results.summary.csv`
delimited by last name, first name, github login, grade (if available), and failure.

### The grade output

The output of the grading scripts are located in `results/<HW>/<login>.out`,
which contains the accumulated sys output from the scripts, which will 
look something like this:

```
POINTS: 561
[ RUN      ] MapKeywordTests/MapKeywordTests.CheckNumInstanceCorrect/160
100
Question breakdown: 
[ 100 100 100 100 100 ]
[       OK ] MapKeywordTests/MapKeywordTests.CheckNumInstanceCorrect/160 (16 ms)
POINTS: 562
[ RUN      ] MapKeywordTests/MapKeywordTests.CheckNumInstanceCorrect/161
100
Question breakdown: 
[ 100 100 100 100 100 ]
[       OK ] MapKeywordTests/MapKeywordTests.CheckNumInstanceCorrect/161 (16 ms)
[==========] 563 tests from 8 test cases ran. (6776 ms total)
[  PASSED  ] 563 tests.

HOMEWORK_GRADE: 563/563
```

Students have reported positively when uploading this file on Canvas along 
with their grade. Verbose tests names helps the students recognize
where they went astray.
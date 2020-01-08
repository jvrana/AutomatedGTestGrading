#!/bin/bash

STUDENTDIR="tmp"                    # the output directory for student repos
CLASSREPO="ECE590"                  # the directory for the class repo
STUDENTREPO="ECE590"                # the name of the student's repo
DIR=$PWD                            # current working directory
RESULTS="$DIR/results"              # output directory for results
DUEDATE=""                          # the due date of the homework

GRADING=$PWD/grading                # path to grading directory, should contain makefile, main, and unit_test
TESTVER=""
MAKE="MakefileGrade$TESTVER"        # name of the makefile to use for compiling
TEST="unit_tests_grading*.c"        # name of the unit_test file
MAIN="main_grading.c"               # name of the main file for tests

SUMMARY="$RESULTS/summary.csv"
GRADEPATTERN="HOMEWORK_GRADE:" # pattern to look for from main.c to build the summary

###### OPTIONS ######
while getopts i:h:l:v:a:d: option
do
case "${option}"
in
i) input=${OPTARG};;    # the input file for student logins
h) HWDIR=${OPTARG};;    # this weeks homework directory name
l) login=${OPTARG};;    # optionally provide a single login to evalute just one student
v) TESTVER=${OPTARG};;
a) APPEND=${OPTARG};;   # if 1, appends result to tmp and results folders
d) DUEDATE=${OPTARG};;  # due date for the homework
esac
done
shift $((OPTIND -1))

function usage() {
    echo "Usage:"
    echo "-h   Which homework you are evaluting (e.g. 'HW_1')"
    echo "-l   Student's github login (optional)"
    echo "-i   Filepath of csv of all students [LAST_NAME,FIRST_NAME,GITHUB_LOGIN]"
    echo "-a   If 1, append student results to results dictionary. If 0, rm -rf results dictionary"
    echo "-d   The due date for the assignment e.g. '2019-01-21'"
}

if ! [[ $HWDIR ]];
then
    echo "OPPS! The argument '-h' missing. Please add the '-h' argument to specify with homework you are evaluating."
    usage
    exit 1
fi

function no_white_space() {
    NO_WHITESPACE="$(echo "${1}" | tr -d '[:space:]')"
    echo $NO_WHITESPACE
}

function evaluate() {
  echo "\nEvaluating $1 $2 ($3)"
  lname=$(no_white_space $1)
  fname=$(no_white_space $2)
  login=$(no_white_space $3)


  cd $DIR
  OUTDIR="${RESULTS}/${HWDIR}"
  mkdir -p $OUTDIR
  OUT="${OUTDIR}/${login}.out"
  echo "Student : ${fname} ${lname} (${login})" #> $OUT
  echo "Github  : ${login}" #>> $OUT
  echo "Course  : ${CLASSREPO}" #>> $OUT
  echo "Homework: ${HWDIR}" #>> $OUT
  echo "\nEVALUATION:" #>> $OUT

  # checkout code before due date
  echo "Checking out master branch before due date $DUEDATE"
  curr=$PWD
  cd $STUDENTDIR/$login
  git checkout "`git rev-list master -n 1 --first-parent --before=$DUEDATE --date=local`"
  cd $curr

  STUDENTMAIN=$STUDENTDIR/$login/$HWDIR/main.cc
  echo $STUDENTMAIN
  if [[ -e $STUDENTMAIN ]];
  then
    STUDENTTARGET=$STUDENTDIR/$login/$HWDIR
  fi

  echo "INFO ($login): $STUDENTTARGET"

  if [[ -e $STUDENTTARGET ]];
  then
    echo "INFO ($login): Found homework directory $STUDENTTARGET"

    # copy all grading files to students directory
    echo "Coping grading file to $STUDENTTARGET"
    cd $STUDENTTARGET
    cp $GRADING/$HWDIR/* .

    # create a new docker container
    echo "Creating docker container..."
    CONTAINERID="$(docker run -v $PWD:/source -di klavins/ecep520:cppenv)"
    echo "Docker container created with id $CONTAINERID"

    # does it compile?
    echo "\n=== COMPILES? ===" >> $OUT
    echo "INFO ($login): Checking compilation"
    docker exec $CONTAINERID make -f $MAKE spotless >> $OUT
    docker exec $CONTAINERID make -f $MAKE >> $OUT
    failure="$(grep -i "failed" $OUT)"

    # does it pass the tests
    echo "\n=== PASSES TESTS? ===" >> $OUT
    echo "INFO ($login): Checking compilation"
    docker exec $CONTAINERID ./bin/test >> $OUT




    # save summary of grades
    grade="$(grep -i $GRADEPATTERN $OUT | cut -d' ' -f 2)"

    echo "Force removing container $CONTAINERID"
    docker rm -f $CONTAINERID
  else
    echo "Homework directory '$STUDENTTARGET' not found!"
    errmsg="ERROR: Homework directory $STUDENTTARGET not found"
    failure=$errmsg
    echo $errmsg >> $OUT
  fi

  echo "$fname,$lname,$login,$grade,$failure" >> $SUMMARY
}

###### EVALUATION ######
echo "***** BEGIN EVALUATION *****"
if [[ $APPEND == 1 ]];
then
    [[ -e $STUDENTDIR ]] && rm -rf $STUDENTDIR
    [[ -e $RESULTS ]] && rm -rf $RESULTS
    touch $SUMMARY
fi
if [[ $input ]];
then
    echo "Reading '${input}'"
    while IFS=',' read fname lname login
    do
      echo "Login $login"
      evaluate $lname $fname $login
    done < "$input"
else
    echo "Using single login '$login'"
    evaluate "unknown" "unknown" $login
fi

echo "***** END EVALUATION *****"

echo "Don't forget to run 'docker system prune' to remove extra containers"

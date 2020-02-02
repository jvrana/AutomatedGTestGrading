#!/bin/bash

STUDENTDIR="tmp"            # the output directory for student repos
CLASSREPO="ECE590"          # the directory for the class repo
STUDENTREPO="AutomatedGTestGradingExample"        # the name of the student's repo
DIR=$PWD                    # current working directory
RESULTS="$DIR/results"      # output directory for results
DUEDATE=""           # the due date of the homework

MAKE="MakefileGrade$TESTVER"        # name of the makefile to use for compiling

SUMMARY="$RESULTS/summary.csv"
GRADEPATTERN="HOMEWORK_GRADE:" # pattern to look for from main.c to build the summary

###### OPTIONS ######
while getopts i:l:d: option
do
case "${option}"
in
i) input=${OPTARG};;  # the input file for student logins
l) login=${OPTARG};;  # optionally provide a single login to evalute just one student
d) DUEDATE=${OPTARG};; # due date for the homework
esac
done
shift $((OPTIND -1))

function usage() {
    echo "Usage:"
    echo "-l   Student's github login (optional)"
    echo "-i   Filepath of csv of all students [LAST_NAME,FIRST_NAME,GITHUB_LOGIN]"
    echo "-d   The due date for the assignment e.g. '2019-01-21'"
}

###### FUNCTIONS ######
function pull_repo () {
    directory=$1
    repo=$2
    echo ""
    echo "Pulling repo '$1'"

    if [[ -e $1 ]];
    then
        echo "INFO: '$directory' already exists. Attempting to fetch and prune..."
        PREVDIR=$PWD
        cd $1
        git fetch origin --prune #> git.log 2>&1
        success=$?
        if [[ $success -eq 0 ]];
        then
            echo "INFO: Successfully fetched and pruned the repo"
        else
            echo "ERROR: There was an error trying to fetch the repo"
        fi
        cd $PREVDIR
    else
        echo "INFO: '$directory' does not exist. Attempting clone repo..."
        git clone "https://github.com/$repo" $directory # > git.log 2>&1
        success=$?
        if [[ $success -eq 0 ]];
        then
            echo "INFO: Repo '$repo' successfully cloned"
        else
            echo "ERROR: Failed to pull repo '$repo'"
        fi
    fi
}

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
  echo "Student : ${fname} ${lname} (${login})"
  echo "Github  : ${login}"
  echo "\nPULL:"

  pull_repo $STUDENTDIR/$login $login/$STUDENTREPO
}

###### SETUP ######
echo "***** SETUP *****"
mkdir -p $RESULTS
pull_repo $CLASSREPO "klavins/ECEP520"
echo "***** END SETUP *****"
echo ""


###### PULL ######
echo "***** BEGIN PULL *****"
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

echo "***** END PULL *****"

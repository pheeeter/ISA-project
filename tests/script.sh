#!/bin/bash

NORMAL=$(tput sgr0)
GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
COUNT_ALL=0
COUNT_PASSED=0
COUNT_FALSE=0
TEST_NAME=""
ERROR=""

echo -e "Testing dns resolver...\n"
echo -e "${GREEN}[-----------]"

#echo "TEST WRONG ARGUMENTS:"

TEST_NAME="./dns -s dns.google -p -1"
echo -e "${GREEN}[ RUN       ]\t${NORMAL}${TEST_NAME}"
./dns -s dns.google -p -1
RC=$?

if [ "${RC}" = 1 ]
then
    echo -e "${GREEN}[        OK ]\t${NORMAL}${TEST_NAME}"
    ((COUNT_PASSED+=1))
else
    echo -e "${RED}[    FAILED ]\t${NORMAL}${TEST_NAME}"
    ((COUNT_FAILED+=1))
fi

((COUNT_ALL+=1))
echo -e "${GREEN}[-----------]\t${NORMAL}${COUNT_ALL} Tests ${COUNT_FALSE} Failed ${COUNT_PASSED} Passed\n"


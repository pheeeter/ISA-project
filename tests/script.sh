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
echo -e "${GREEN}[-----------]${NORMAL}\t    TESTING PARAMETERS"

#echo "TEST WRONG ARGUMENTS:"

TEST_NAME="./dns -s dns.google -p -1"
echo -e "${GREEN}[ RUN       ]\t${NORMAL}${TEST_NAME}"
./dns -s dns.google -p -1 2>/dev/null
RC=$?

if [ "${RC}" = 1 ]
then
    echo -e "${GREEN}[        OK ]"
    ((COUNT_PASSED+=1))
else
    echo -e "${RED}[    FAILED ]\t${NORMAL}${TEST_NAME} [RC=${RC} EXPECTED=1]"
    ((COUNT_FAILED+=1))
fi

echo -e "${GREEN}[ RUN       ]\t${NORMAL}${TEST_NAME}"
./dns -s dns.google -p 100000 -f tests/refused_domains 2>/dev/null
RC=$?
if [ "${RC}" = 1 ]
then
    echo -e "${GREEN}[        OK ]"
    ((COUNT_PASSED+=1))
else
    echo -e "${RED}[    FAILED ]${NORMAL}\tRC=${RC} EXPECTED=1"
    ((COUNT_FAILED+=1))
fi

echo -e "${GREEN}[-----------]${NORMAL}"
echo -e "${GREEN}[-----------]${NORMAL}\t  NOT IMPLEMENTED QUERIES"
./dns -s dns.google -p 3333 -f tests/refused_domains 2>/dev/null
dig -p 3333 @localhost google.com MX
echo -e "${GREEN}[-----------]\t${NORMAL}"


((COUNT_ALL+=1))
echo -e "\n${COUNT_ALL} Tests ${COUNT_FALSE} Failed ${COUNT_PASSED} Passed\n"


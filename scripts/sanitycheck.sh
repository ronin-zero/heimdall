#!/usr/bin/env bash

UNIQUE=$1
BASE=$2
REGEX="("

for val in `cat $UNIQUE`; 
do 
    REGEX="${REGEX}|${val}"
done; 
REGEX="${REGEX})"; 
egrep -v "${REGEX}" $BASE;

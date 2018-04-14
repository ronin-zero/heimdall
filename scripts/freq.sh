#!/usr/bin/env bash

if [ $# -lt 1 ];
then
    echo "ERROR: Must specify file containing the unique data (unique PIDs, unique syscalls, etc)"
    if [ $# -lt 2 ];
    then
        echo "ERROR: Must specify the original log file to run against."
        exit 2
    fi
    exit 1
fi

UNIQUE=$1
LOG=$2

HIST="${2%.*}.freq"

if [ -f $HIST ];
then
    rm $HIST
fi

for val in `cat $UNIQUE`;
do
    COUNT=$(egrep -c "${val}$" $LOG)
    printf "$val    $COUNT\n" >> $HIST
done

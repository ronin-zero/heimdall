#!/usr/bin/env bash

get_unique() {
    if [ -f "$2" ];
    then
        rm $2
    fi

    echo "Log file is $1 and separator is $3 output file is $2 field to separate is $4"

    for line in `cut -d ${3} -f ${4} ${1} | sort -h`;
    do
        if [ "$line" != "$PREV" ];
        then
            echo $line >> $2
            PREV=$line
        fi
    done
}


if [ $# -lt 1 ];
then
    echo "ERROR: At least one argument required (name of the trace log file)"
    exit 1
fi

LOGFILE=$1

if [ ! -f $LOGFILE ];
then
    echo "ERROR: File $LOGFILE not found!"
    exit 2
fi

if [ $# -ge 2 ];
then
    SEP=$2
else
    SEP=,
fi

PID_OUT="${LOGFILE%%.*}.pid"
SYSCALL_OUT="${LOGFILE%%.*}.syscall"

get_unique $LOGFILE $PID_OUT $SEP 2
get_unique $LOGFILE $SYSCALL_OUT $SEP 4

exit 0

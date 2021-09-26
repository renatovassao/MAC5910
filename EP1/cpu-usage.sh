#!/bin/bash

set -eu

PORT="${1:--1}"
LOGFILE="tmp.log"

./check.sh $PORT

if [ -f "$LOGFILE" ]; then
    read -p "$LOGFILE exists, replace it ? [Y/n]: " INPUT

    if [[ "$INPUT" == "n" || $INPUT == "N"  ]]; then
        echo "aborting"
        exit
    fi

    rm -v $LOGFILE
fi

i=1
while true; do
    echo "$i $(top -b -n1 | grep mqtt | awk '{cpu+=$9} END {print cpu}')"
    # top "${PIDS[@]/#/-p }" -b -n 1 >> $LOGFILE
    sleep 1
    ((i++))
done

#  | grep mqtt | awk '{cpu+=$9} END {print cpu}'
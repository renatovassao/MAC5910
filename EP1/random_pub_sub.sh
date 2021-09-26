#!/bin/bash

set -eu

PORT="${1:--1}"
NPROCS="${2:-2}"

if [ $((NPROCS % 2)) -ne 0 ]; then
    echo "nprocs must be even"
    echo "usage: $0 <port> <nprocs>"
    exit 1
fi

./check.sh $PORT

trap 'kill $(jobs -p); pkill mosquitto_sub' EXIT


NTOPICS=$(( (RANDOM % (NPROCS / 2)) + 1))
MAXSLEEP=1
MAXLENGTH=100
RUNTIME=60

echo "using nprocs    = $NPROCS"
echo "using ntopics   = $NTOPICS"
echo "using maxsleep  = $MAXSLEEP"
echo "using maxlength = $MAXLENGTH"

TOPICS=($(for i in $(seq 1 "$NTOPICS"); do echo "topic_$i"; done))

random_sub() {
    INDEX=$((RANDOM % NTOPICS))
    TOPIC=${TOPICS[$INDEX]}
    echo "subscribing to $TOPIC"
    mosquitto_sub -t $TOPIC -p $PORT > /dev/null
}

random_pub() {
    while true; do
        SLEEP=$(( (RANDOM % MAXSLEEP) + 1))
        INDEX=$((RANDOM % NTOPICS))
        TOPIC=${TOPICS[$INDEX]}
        LENGTH=$(( (RANDOM % MAXLENGTH) + 1))
        MESSAGE=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $LENGTH | head -n 1)
        echo "publishing '${MESSAGE:0:10}... ($LENGTH)' to $TOPIC"
        mosquitto_pub -t $TOPIC -m "$MESSAGE" -p $PORT
        sleep $SLEEP
    done
}

for i in $(seq 1 $((NPROCS / 2))); do
    random_sub &
done

for i in $(seq 1 $((NPROCS / 2))); do
    random_pub &
done

sleep $RUNTIME
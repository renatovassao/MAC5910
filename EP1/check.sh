#!/bin/bash

set -eu

PORT="${1:--1}"

if [ "$PORT" == -1 ]; then
    echo "missing port"
    echo "usage: $0 <port>"
    exit 1
fi

if [ -z "$(lsof -i tcp:$PORT | head -n1)" ]; then
    echo "no process running at $PORT"
    exit 1
fi
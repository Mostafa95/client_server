#!/usr/bin/bash

# This script test whether client is up and running

tag=($( tail -n 2 ./build/client_reading.txt ))
logTime=${tag[3]}

now=$(date +"%T")

diff=$(( $(date -d "$now" "+%s") - $(date -d "$logTime" "+%s") ))

if [[ $diff -ge 5 ]];
then
    echo "client is down"
else
    echo "client is running"
fi
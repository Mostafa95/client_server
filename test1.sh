#!/usr/bin/bash

#This script test server status

if (  nc -vz $1 $2 )
then
    echo "server is up and running" 
else
    echo "server is down" 
fi

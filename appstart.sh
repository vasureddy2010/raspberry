#!/bin/bash

pgrep SMARTTRAK > /dev/null
if [ $? -eq 0 ]; then
  echo "APPLICATION PROCESS IS RUNNING"
else
  echo "APPLICATION PROCESS IS NOT RUNNING"
  cd  /home/pi/Documents/TRK/ && ./SMARTTRAK > /dev/null &
fi

pgrep smTrkCli_ACDB_MFM  > /dev/null
if [ $? -eq 0 ]; then
  echo "APPLICATION PROCESS IS RUNNING"
else
  echo "APPLICATION PROCESS IS NOT RUNNING"
  cd  /home/pi/Documents/ENERGY/ && ./smTrkCli_ACDB_MFM > /dev/null &
fi


pgrep WATCHDOG > /dev/null
if [ $? -eq 0 ]; then
  echo "WATCHDOG PROCESS IS RUNNING"
else
  echo "WATCHDOG PROCESS IS NOT RUNNING"
  cd  /home/pi/SMARTTRAK/ && ./WATCHDOG  > /dev/null &
fi

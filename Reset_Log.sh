#!/bin/bash

FILESIZE=$(stat -c%s "Reset_Log.txt")
Size_Limit=5120
f=`expr $FILESIZE + 0`
if [ $f -ge $Size_Limit ] 
then 
  printf "Out of file-size-limit($Size_Limit Bytes):::first-line-detlete\n"
  sed -i '1d' Reset_Log.txt
fi

{ printf "Reset:" & date; } >> Reset_Log.txt




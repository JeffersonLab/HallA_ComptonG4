#!/bin/bash
prefix="scintyield"

if [ "x$1" == "x" ];
then
  echo "Did not specify start run number"
  exit;
fi

if [ "x$2" == "x" ];
then
  echo "Did not specify end run number"
  exit;
fi

if [ "x$3" == "x" ];
then
  echo "Did not specify Setting number (1-3)"
  exit;
fi

for i in `seq $1 $2`;
do
  cp ${prefix}.xml pbs/${prefix}_${3}_${i}.xml
  sed -i -e 's/SETTING/'${i}'/g' -- pbs/${prefix}_${3}_${i}.xml
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/${prefix}_${3}_${i}.xml
  #jsub -xml pbs/${prefix}_$i.xml
  #cat ${prefix}_$i.xml
  #rm ${prefix}_$i.xml
done

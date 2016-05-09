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
  echo "Did not specify Setting number (1-4)"
  exit;
fi

for i in `seq -f %05g $1 $2`;
do
  file_base="pbs/${prefix}_${3}_${i}"
  xml_file="${file_base}.xml"
  cp ${prefix}.xml ${xml_file}
  sed -i -e 's/SETTING/'${3}'/g' -- ${xml_file}
  sed -i -e 's/RUNNUM/'${i}'/g' -- ${xml_file}
  jsub -xml ${xml_file}
  #cat ${prefix}_$i.xml
  #rm ${prefix}_$i.xml
done

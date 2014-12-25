#!/bin/bash

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

for i in `seq -f %05g $1 $2`;
do
  cp sim_2thin_lead.mac pbs/sim_2thin_lead_${i}.mac
  cp sim_2thin_lead.xml pbs/sim_2thin_lead_${i}.xml
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_2thin_lead_$i.mac
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_2thin_lead_$i.xml
  jsub -xml pbs/sim_2thin_lead_$i.xml
  #cat sim_2thin_lead_$i.xml
  #rm sim_2thin_lead_$i.xml
done

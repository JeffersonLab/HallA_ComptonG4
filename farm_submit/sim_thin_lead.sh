#!/bin/bash

for i in `seq 7001 7499`;
do
  cp sim_thin_lead.mac pbs/sim_thin_lead_${i}.mac
  cp sim_thin_lead.xml pbs/sim_thin_lead_${i}.xml
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_thin_lead_$i.mac
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_thin_lead_$i.xml
  jsub -xml pbs/sim_thin_lead_$i.xml
  #cat sim_thin_lead_$i.xml
  #rm sim_thin_lead_$i.xml
done

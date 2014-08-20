#!/bin/bash

for i in `seq 8000 8099`;
do
  cp sim_2thin_lead.mac pbs/sim_2thin_lead_${i}.mac
  cp sim_2thin_lead.xml pbs/sim_2thin_lead_${i}.xml
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_2thin_lead_$i.mac
  sed -i -e 's/RUNNUM/'${i}'/g' -- pbs/sim_2thin_lead_$i.xml
  jsub -xml pbs/sim_2thin_lead_$i.xml
  #cat sim_2thin_lead_$i.xml
  #rm sim_2thin_lead_$i.xml
done

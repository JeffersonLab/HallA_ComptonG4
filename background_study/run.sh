#!/bin/bash
if [ "x$1" == "x" ];
then
  exit;
fi
RUN=`printf "%03d" $1`

printf "\033kRun${RUN}\033\\"
cp background_study/background_study_general.mac background_study/background_study_run${RUN}.mac
sed -i -e 's/RUNNUM/'${RUN}'/g' -- background_study/background_study_run${RUN}.mac
ComptonG4_batch \
  --batch-file=background_study/background_study_run${RUN}.mac \
  --rootfile-prefix=ComptonG4_background_study \
  --geometry-file=geometries/ComptonGeometry_2thin_lead_mag.xml \
  --output-dir=background_study/rootfiles \
  --enable-optical=false \
  --random-seed=3829${RUN}

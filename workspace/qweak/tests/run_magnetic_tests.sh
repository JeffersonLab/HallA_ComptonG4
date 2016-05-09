#!/bin/bash
## Configure what to run here
SCANDIPOLE=true
SCANTRANSVERSE=false

DEFAULTRUN=999999
DEFAULTX1=0.0
DEFAULTY1=0.0
DEFAULTZ1=117.5
DEFAULTZ1=112.15

DEFAULTX2=0.0
DEFAULTY2=57.0
DEFAULTZ2=437.0

DEFAULTTRANS=0.0

function prepfiles()
{
  cp tests/run_magnetic_tests.mac tests/current_magnetic_test.mac
  cp tests/run_magnetic_tests.xml geometries/MagneticDefinitions.xml
}

function setdefaults()
{
  ## Run number
  sed -i -e 's/RUNNUM/'${DEFAULTRUN}'/g' -- tests/current_magnetic_test.mac
  ## Transverse polarization
  sed -i -e 's/SETTRANS/'${DEFAULTTRANS}'/g' -- tests/current_magnetic_test.mac
  ## Dipole positions
  sed -i -e 's/SETX1/'${DEFAULTX1}'/g' -- geometries/MagneticDefinitions.xml
  sed -i -e 's/SETY1/'${DEFAULTY1}'/g' -- geometries/MagneticDefinitions.xml
  sed -i -e 's/SETZ1/'${DEFAULTZ1}'/g' -- geometries/MagneticDefinitions.xml
  sed -i -e 's/SETX2/'${DEFAULTX2}'/g' -- geometries/MagneticDefinitions.xml
  sed -i -e 's/SETY2/'${DEFAULTY2}'/g' -- geometries/MagneticDefinitions.xml
  sed -i -e 's/SETZ2/'${DEFAULTZ2}'/g' -- geometries/MagneticDefinitions.xml
}

function runcmd()
{
  paddedrun=`seq -f "%05g" $1 $1`
  ComptonG4_batch -c configs/run_magnetic_tests.cfg &> tests/logs/ComptonG4_magnetic_tst_${paddedrun}.log
}


if [ "x$SCANDIPOLE" == "xtrue" ];
then
  run=0
  #for pos in `seq --format="%f" 112.15 0.01 112.25`;
  for pos in `seq --format="%f" 112.0 0.05 119.0`;
  do
    let run+=1
    echo "Position ${pos} cm\t Run: ${run}"
    prepfiles
    sed -i -e 's/RUNNUM/'${run}'/g' -- tests/current_magnetic_test.mac
    sed -i -e 's/SETZ1/'${pos}'/g' -- geometries/MagneticDefinitions.xml
    setdefaults
    runcmd ${run}
  done
fi

if [ "x$SCANTRANSVERSE" == "xtrue" ];
then
  run=100
  for trans in `seq --format="%f" 0.0 0.1 1.0`;
  do
    let run+=1
    echo "Transverse: ${trans}, Run: ${run}"
    prepfiles
    sed -i -e 's/RUNNUM/'${run}'/g' -- tests/current_magnetic_test.mac
    sed -i -e 's/SETTRANS/'${trans}'/g' -- tests/current_magnetic_test.mac
    setdefaults
    runcmd ${run}
  done
fi

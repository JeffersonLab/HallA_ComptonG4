#!/bin/bash

################################################################################
## Process Command Line Options
for i in $*
do
  case $i in
    ## This flag specifies run range
    --runs=*)
      RUNS=`echo $i | sed 's/--runs=//'`
      RUNMIN=${RUNS%:*}
      RUNMAX=${RUNS#*:}
    ;;
    ## This flag is to only run the specified macros
    --setting=*)
      SETTING=`echo $i | sed 's/--setting=//'`
    ;;
    ## This flag changes the number of events
    --events=*)
      EVENTS=`echo $i | sed 's/--events=//'`
    ;;
    ## This flag will produce all files, but will not submit anything
    --testmacro)
      TESTMACRO=1
    ;;
    ## This flag will submit these runs as tests and use the test macros
    --testrun)
      TESTRUN=1
    ;;

    *)
      echo "Error: option $i is unkown"
      exit -1;
    ;;
  esac
done

############################################
### Now verify settings

number='^[0-9]+$'
## Ensure run range was specified
if [ -z "${RUNS}" ];
then
  echo "Must specify run range via --runs=low:high"
  exit -1
## But are they numbers
elif ! [[ ${RUNMIN} =~ ${number} ]] || ! [[ ${RUNMAX} =~ ${number} ]];
then
  echo "Run range ${RUNS} invalid. Check your syntax."
  exit -1
elif [[ ${RUNMIN} -gt ${RUNMAX} ]];
then
  echo "Min run must be larger than max run. Range specified ${RUNS}"
  exit -1
else
  echo "Will submit $((${RUNMAX}-${RUNMIN}+1)) run(s) from ${RUNMIN} to ${RUNMAX}"
fi

## Check that a setting was specified
if [ -z "${SETTING}" ];
then
  echo "No setting specified! Specify setting with --setting=<setting_name>"
  exit -2
else
  echo "Setting set to ${SETTING}"
fi

## Do we want to submit a test run?
if [ -z ${TESTRUN} ]; # Normal running
then
  TIMEMINUTES="780"
  TRACKNAME="simulation"
  MACROFILE="sim_${SETTING}.mac"
else
  TIMEMINUTES="30"
  TRACKNAME="test"
  MACROFILE="sim_test_${SETTING}.mac"
  echo "    *** TEST RUNS ONLY ***"
fi


## Ensure that geometry file and macro file exist
if ! [ -e "../geometries/ComptonGeometry_${SETTING}.xml" ];
then
  echo "Error geometry file does not exist! ../geometries/ComptonGeometry_${SETTING}.xml"
  exit -3
elif ! [ -e "${MACROFILE}" ];
then
  echo "Error macro file ${MACROFILE} does not exist"
  exit -4
fi

## Configure number of events and check that it is still a number
#${EVENTS:=2500}
#if ! [[ ${EVENTS} =~ ${number} ]];
#then
#  echo "Error! Events specified is not a number: ${EVENTS}"
#  exit -5
#elif [ -z "${TESTRUN}" ];
#then
#  EVENTS=100
#fi

for RUN in `seq -f %05g ${RUNMIN} ${RUNMAX}`;
do
  XMLFILE="pbs/sim_${SETTING}_${RUN}.xml"
  cp sim_submit.xml ${XMLFILE}
  sed -i -e 's/SETTING/'${SETTING}'/g' -- ${XMLFILE}
  sed -i -e 's/RUNNUM/'${RUN}'/g' -- ${XMLFILE}
  sed -i -e 's/TIMEMINUTES/'${TIMEMINUTES}'/g' -- ${XMLFILE}
  sed -i -e 's/TRACKNAME/'${TRACKNAME}'/g' -- ${XMLFILE}
  sed -i -e 's/MACROFILE/'${MACROFILE}'/g' -- ${XMLFILE}
  #sed -i -e 's/EVENTS/'${EVENTS}'/g' -- ${XMLFILE}
  if [ -z ${TESTMACRO} ];
  then
    jsub -xml ${XMLFILE}
  else
    echo "Test only: jsub -xml ${XMLFILE}"
  fi
  #cat ${prefix}_$i.xml
  #rm ${prefix}_$i.xml
done

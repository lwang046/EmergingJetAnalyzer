#!/bin/bash

STARTINGDIR=${PWD}
echo $STARTINGDIR
# cd cogFiles
export PYTHONPATH=${PWD}/cogFiles
declare -a FILES_TO_COG=("EmergingJetAnalyzer/plugins/EmergingJetAnalyzer.cc" "EmergingJetAnalyzer/interface/OutputTree.h" "EmJetAnalyzer/interface/OutputTree.h" "EmJetAnalyzer/interface/EmJetEvent.h")
for file in "${FILES_TO_COG[@]}"
do
    # echo "Trying to cog file: ${file}"
    # if [[ `git status --porcelain $file` ]]; then
    #     echo "${file} has been changed! Commit before building"
    #     exit 1
    # else
        # echo cog
        echo "cog.py --verbosity=2 -r ${STARTINGDIR}/${file}"
        cog.py --verbosity=2 -r "${STARTINGDIR}/${file}"
    # fi
done
# echo "WARNING: Replacing EmergingJetAnalyzer/interface/OutputTree.h"
# cog.py -r ${STARTINGDIR}/EmergingJetAnalyzer/interface/OutputTree.h
# echo "WARNING: Replacing EmergingJetAnalyzer/plugins/EmergingJetAnalyzer.cc"
# cog.py -r ${STARTINGDIR}/EmergingJetAnalyzer/plugins/EmergingJetAnalyzer.cc
#USER_CXXFLAGS="-Wno-error=unused-variable -Wno-error=unused-but-set-variable -DEDM_ML_DEBUG -g" scram b -v -j8
#cd $STARTINGDIR

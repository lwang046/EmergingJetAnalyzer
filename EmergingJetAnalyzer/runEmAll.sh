#! /bin/bash
cmsRun test/modelA.py &
cmsRun test/modelB.py &
cmsRun test/modelC.py &
cmsRun test/modelD.py &
cmsRun test/qcd_relval.py


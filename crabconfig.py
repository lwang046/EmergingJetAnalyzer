from CRABClient.UserUtilities import config
config = config()

import time

config.section_("General")
config.General.workArea = 'crabProjects/GJetMC'
config.General.requestName = 'crab_GJetMC_HT600TOINF' + time.strftime("-%Y-%m%d") + '_v1'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'Configuration/test/test_gjet_cfg.py'

config.section_("Data")
config.Data.outputPrimaryDataset = 'GJetMC_HT600TOINF'
config.Data.userInputFiles = ['/store/mc/RunIIFall15DR76/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/183FF102-C896-E511-9401-20CF305B0521.root']
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
config.Data.outputDatasetTag = config.General.requestName

config.section_("Site")
config.Site.storageSite = "T3_US_UMD"

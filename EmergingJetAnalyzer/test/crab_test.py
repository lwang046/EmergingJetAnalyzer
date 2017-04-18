from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")
config.General.requestName   = 'QCD_may22'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName  = 'ANALYSIS'
# Name of the CMSSW configuration file
config.JobType.psetName    = 'EmergingJetsAnalyzer_test.py'

config.section_("Data")
config.Data.inputDataset = '/QCD_HT_1000ToInf_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1/AODSIM'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/tkolberg'
# This string is used to construct the output dataset name
#config.Data.publishDataName = 'CRAB3-tutorial'

# These values only make sense for processing data
#    Select input data based on a lumi mask
#config.Data.lumiMask = 'Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
#    Select input data based on run-ranges
#config.Data.runRange = '190456-194076'

config.section_("Site")
# Where the output files will be transmitted to
config.Site.storageSite = 'T2_CH_CERN'

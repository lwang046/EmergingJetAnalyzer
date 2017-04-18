
# Specify datasets by a shorthand alias, full name, units per job and total units (EventAwareLumi/Event for MC, Lumi for data)
# (alias, Data.inputDataset, Data.unitsPerJob, Data.totalUnits, isData, JobType.priority, sampleType)
class dataset:
    """Simple class to hold information relevant to a given dataset for CRAB jobs.
    label is unused member that can be used to hold extra information specific to each MultiCRAB config.
    E.g. dataset("WJetsToLNuInclusive", "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM", MC, 1000, 1000000, 10, 'EventBased', 'WJet')"""
    def __init__(self, alias, fullpath, isData, unitsPerJob=1, totalUnits=1, splitting='FileBased', priority=1, inputDBS='global', label=''):
        self.alias       = alias
        self.fullpath    = fullpath
        self.isData      = isData
        self.unitsPerJob = unitsPerJob
        self.totalUnits  = totalUnits
        self.splitting   = splitting
        self.priority    = priority
        self.inputDBS    = inputDBS
        self.label       = label

# datasets = [
# dataset( "test" , "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , 1 , 1 ) ,
# dataset( "WJetsToLNuInclusive" , "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"   , 50000 , 50000000 , MC , 15 ) ,
# dataset( "QCD_Pt_5to10"        , "/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"              , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_10to15"       , "/QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"             , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_15to30"       , "/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"             , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_30to50"       , "/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"             , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_50to80"       , "/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"             , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_80to120"      , "/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"            , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_120to170"     , "/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"           , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_170to300"     , "/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"           , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_300to470"     , "/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"           , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_470to600"     , "/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"           , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_600to800"     , "/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/AODSIM"           , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_Pt_800to1000"    , "/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"          , 50000 , 10000000 , MC , 10 ) ,
# dataset( "QCD_HT100to200"   , "/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"   , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT200to300"   , "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"   , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT300to500"   , "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"   , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT500to700"   , "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"   , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT700to1000"  , "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"  , 1 , 1000 , MC , 13 ) ,
# dataset( "QCD_HT1000to1500" , "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM" , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT1500to2000" , "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , 1 , 100  , MC , 13 ) ,
# dataset( "QCD_HT2000toInf"  , "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"  , 1 , 100  , MC , 13 ) ,
# dataset
# dataset( "WJetsToLNu_HT-100To200"   , "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"   , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-200To400"   , "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"   , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-400To600"   , "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/AODSIM"   , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-600To800"   , "/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"   , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-800To1200"  , "/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"  , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-1200To2500" , "/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , 10000, 1000000 , MC) ,
# dataset( "WJetsToLNu_HT-2500ToInf"  , "/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"  , 10000, 1000000 , MC) ,
# dataset( "SingleMuonB"     , "/SingleMuon/Run2015B-PromptReco-v1/AOD"     , 1 , 100000 , DATA , 20 ) ,
# dataset( "SingleElectronB" , "/SingleElectron/Run2015B-PromptReco-v1/AOD" , 1 , 100000 , DATA , 20 ) ,
# dataset( "SingleMuonD-PRv3"     , "/SingleMuon/Run2015D-PromptReco-v3/AOD"     , 10 , 100000 , DATA , 20 ) ,
# dataset( "SingleElectronD-PRv3" , "/SingleElectron/Run2015D-PromptReco-v3/AOD" , 10 , 100000 , DATA , 20 ) ,
# dataset( "SingleMuonD-PRv4"     , "/SingleMuon/Run2015D-PromptReco-v4/AOD"     , 10 , 100000 , DATA , 20 ) ,
# dataset( "SingleElectronD-PRv4" , "/SingleElectron/Run2015D-PromptReco-v4/AOD" , 10 , 100000 , DATA , 20 ) ,
# dataset( "ModelA"       , "/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/yoshin-AODSIM-69070e02f00a6fbca8a74a9d93177037/USER" , 100 , 10000000 , MC   , 20 , 'signal' ) ,
# dataset( "ModelB"       , "/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/yoshin-AODSIM-69070e02f00a6fbca8a74a9d93177037/USER" , 100 , 10000000 , MC   , 20 , 'signal' ) ,
# dataset( "WJetSkimMuon" , "/SingleMuon/yoshin-WJetSkim-ede3f21fae18a825b193df32c86b780e/USER"                                      , 10 , 1000000 , DATA , 20 , 'wjet'   ) ,
# ]

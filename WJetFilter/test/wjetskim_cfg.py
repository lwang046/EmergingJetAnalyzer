############################################################
# Important things to change before running
# - Default value for inputFiles
# - Global Tag (between releases)
# - datasets.py import location
# - data/MC
############################################################
# GlobalTag switching based on version of CMSSW
# For each version, specify global tag for MC and data as a list
# e.g. ['MCRUN2_74_V9', '74X_dataRun2_Prompt_v2']
import os
cmssw_version = os.environ['CMSSW_VERSION']
if 'CMSSW_7_4_12' in cmssw_version:
    globalTags=['74X_mcRun2_design_v2','74X_dataRun2_Prompt_v2']
elif 'CMSSW_7_4_1_patch4' in cmssw_version:
    globalTags=['MCRUN2_74_V9','74X_dataRun2_Prompt_v0']
print 'CMSSW_VERSION is %s' % cmssw_version
print 'Using the following global tags [MC, DATA]:'
print globalTags


import FWCore.ParameterSet.Config as cms
# Command line argument parsing
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')


## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

## Options and Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False),
        # SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.maxEvents = -1 # -1 means all events
# options.inputFiles= 'root://xrootd-cms.infn.it//store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/AODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00CC714A-F86B-E411-B99A-0025904B5FB8.root'
# options.inputFiles= 'file:/afs/cern.ch/work/y/yoshin/public/RunIISpring15DR74/DYJetsToLL_M-50_13TeV-madgraph-pythia8/AODSIM/00CC714A-F86B-E411-B99A-0025904B5FB8.root'
# options.inputFiles= '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150715_195547/0000/aodsim_10.root'
options.inputFiles= '/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/96EF1A5F-8115-E511-AF17-02163E0125CE.root'
# options.inputFiles= '/store/mc/RunIISpring15DR74/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/60000/78519985-C817-E511-BDD1-008CFA0A565C.root'
# options.inputFiles= '/store/mc/RunIISpring15DR74/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/10198812-0816-E511-A2B5-AC853D9DAC1D.root'
# options.inputFiles= '/store/data/Run2015B/SingleMuon/AOD/PromptReco-v1/000/251/168/00000/02132736-CB26-E511-8127-02163E01386E.root'
options.outputFile = ''
options.register ('crab',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to run on CRAB.")
options.crab = 0
options.register ('data',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 for data.")
options.data = 0


# get and parse the command line arguments
options.parseArguments()


# Unscheduled mode
# process.options.allowUnscheduled = cms.untracked.bool(False)
process.options.allowUnscheduled = cms.untracked.bool(True)



### =========== Global configuration ==========================

## MET flavor configuration -> may be time consuming
## NoPu and MVA met need to be generated with some refeernce objects (e;g. leptons)
## look for the corresponding area in the config file to set your own definition


### ===========================================================
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTags[options.data], '')

process.source = cms.Source("PoolSource",
		# fileNames = cms.untracked.vstring("file:input.root"),
    skipEvents = cms.untracked.uint32(0)
)
# if options.inputFiles :
process.source.fileNames = cms.untracked.vstring(options.inputFiles)

if options.data and not options.crab:
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename = 'goodList.json').getVLuminosityBlockRange()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
    )

##
## To remove the "begin job processing line " from printing
##
#process.MessageLogger = cms.Service("MessageLogger",
#   destinations = cms.untracked.vstring('cout',),
#   cout         = cms.untracked.PSet(threshold = cms.untracked.string('ERROR'))
#)
# process.MessageLogger.cout.INFO.limit = 0
# process.MessageLogger.cerr.threshold = "DEBUG"
# process.MessageLogger.categories.append('JetFilter')

# process.MessageLogger.cerr.FwkReport.reportEvery = 1
# process.MessageLogger.cerr.default.limit = 100
#process.options.wantSummary = False

############################################################
# PAT Default Configuration
############################################################
# process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
# process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
# process.load("PhysicsTools.PatAlgos.cleaningLayer1.cleanPatCandidates_cff")
# process.load("PhysicsTools.PatAlgos.selectionLayer1.countPatCandidates_cff")

# from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning

############################################################
# Recreate miniAOD
############################################################
if options.data: 
    # customisation of the process.
    process.load('Configuration.StandardSequences.PAT_cff')
    # Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
    from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllData 
    #call to customisation function miniAOD_customizeAllData imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
    process = miniAOD_customizeAllData(process)
else: 
    # customisation of the process.
    process.load('Configuration.StandardSequences.PATMC_cff')
    # Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
    from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 
    #call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
    process = miniAOD_customizeAllMC(process)
############################################################

############################################################
# Private modules
############################################################
process.eventCountPreFilter = cms.EDAnalyzer('EventCounter')
process.eventCountPostFilter = cms.EDAnalyzer('EventCounter')

process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
        'HLT_Ele27_eta2p1_WPLoose_Gsf_v*',
        'HLT_Ele27_eta2p1_WPTight_Gsf_v*',
        'HLT_Ele27_eta2p1_WP75_Gsf_v*',
        'HLT_Ele32_eta2p1_WP75_Gsf_v*',
        'HLT_IsoMu20_v*',
        'HLT_IsoMu20_eta2p1_v*',
        'HLT_IsoMu24_eta2p1_v*',
    ),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "gtDigis" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( False )
)


process.wJetFilter = cms.EDFilter("WJetFilter",
    isData = cms.bool( False ),
    srcMuons = cms.InputTag("slimmedMuons"),
    srcElectrons = cms.InputTag("slimmedElectrons"),
    srcJets = cms.InputTag("slimmedJets"),
    srcMET = cms.InputTag("slimmedMETs"),
    minPtMuon = cms.double(20.0),
    minPtElectron = cms.double(20.0),
    minPtMET = cms.double(20.0),
    minMt = cms.double(50.0),
    maxMt = cms.double(100.0),
    minDeltaR = cms.double(0.4),
    maxDeltaPhi = cms.double(0.4), # Doesn't do anything
    minPtSelectedJet = cms.double(20.0),
    maxPtAdditionalJets = cms.double(20.0), # Doesn't do anything
    # electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-loose'),
    electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium'),
)
if options.data: process.wJetFilter.isData = cms.bool( True )
if 'CMSSW_7_4_12' in cmssw_version:
    process.wJetFilter.electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-medium')
elif 'CMSSW_7_4_1_patch4' in cmssw_version:
    process.wJetFilter.electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium')

process.genJetFilter = cms.EDFilter("GenJetFilter",
    srcJets = cms.InputTag("ak4GenJets"),
)

from TrackingTools.TrackAssociator.default_cfi import *
process.emergingJetAnalyzer = cms.EDAnalyzer('EmergingJetAnalyzer',
    TrackAssociatorParameterBlock,
    srcJets = cms.InputTag("wJetFilter"),
)
# process.jetFilter = cms.EDFilter("JetFilter",
#     srcJets = cms.InputTag("ak4PFJetsCHS"),
#     # srcJets = cms.InputTag("patJets"),
#     # additionalCut = cms.string(""),
#     additionalCut = cms.string("abs(eta) < 2.5 && pt > 50.0"),
#     jetCuts = cms.VPSet(
#         cms.PSet(
#             minPt = cms.double(400.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#         ),
#         cms.PSet(
#             minPt = cms.double(200.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#         ),
#         cms.PSet(
#             minPt = cms.double(125.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#         ),
#         cms.PSet(
#             minPt = cms.double(50.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#         ),
#     )
# )

# process.demo = cms.EDAnalyzer('EmergingJetAnalyzer'
# )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("histo.root")
)

# process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
# process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
#                                    src = cms.InputTag("genParticles"),                                                                 
#                                    printP4 = cms.untracked.bool(False),
#                                    printPtEtaPhi = cms.untracked.bool(True),
#                                    printVertex = cms.untracked.bool(False),
#                                    printStatus = cms.untracked.bool(True),
#                                    printIndex = cms.untracked.bool(False),
#                                    # status = cms.untracked.vint32( 3 )
#                                    )
# process.printDecay = cms.EDAnalyzer("ParticleDecayDrawer",
#     src = cms.InputTag("genParticles"),
#     printP4 = cms.untracked.bool(True),
#     printPtEtaPhi = cms.untracked.bool(True),
#     printVertex = cms.untracked.bool(False)
#   )
#
process.p = cms.Path(
    process.eventCountPreFilter*
    process.triggerSelection*
    process.genJetFilter*
    process.wJetFilter*
    process.eventCountPostFilter
    # process.emergingJetAnalyzer
)

if options.data: 
    process.p = cms.Path(
        process.eventCountPreFilter*
        process.triggerSelection*
        process.wJetFilter*
        process.eventCountPostFilter
    )


############################################################
# PATH definition
############################################################
# process.muonPath = cms.Path( process.countPatMuons )
# process.electronPath = cms.Path( process.countPatElectrons )

# process.jetPath = cms.Path( process.jetFilter )




from Configuration.EventContent.EventContent_cff import AODSIMEventContent
from Configuration.EventContent.EventContent_cff import AODEventContent

process.out = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fileName = cms.untracked.string('file:output.root'),
    outputCommands = cms.untracked.vstring(),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    )
)

if options.data : process.out.outputCommands.extend(AODEventContent.outputCommands)
else            : process.out.outputCommands.extend(AODSIMEventContent.outputCommands)

process.out.outputCommands.extend(
    cms.untracked.vstring(
        'keep *_wJetFilter_*_*',
        # 'keep *_emergingJetAnalyzer_*_*',
    )
)
# process.out.outputCommands = cms.untracked.vstring(
    # 'keep *_genJetFilter_*_*',
    # 'keep *_wJetFilter_*_*',
    # 'keep *_emergingJetAnalyzer_*_*',
# )


if options.outputFile=='.root' or options.outputFile.find('_numEvent')==0:
    print """
############################################################
Warning: outputFile unspecified. Writing to output.root
############################################################
"""
else:
    process.out.fileName = cms.untracked.string(options.outputFile)
    print """
############################################################
Writing to """ + options.outputFile + """
############################################################
"""


process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    categories = cms.untracked.vstring('WJetFilter'),
    debugModules = cms.untracked.vstring('wJetFilter'),
    cout         = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        # DEBUG = cms.untracked.int32(0),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
        ),
    )
)
# storage
process.outpath = cms.EndPath(process.out) #dummy



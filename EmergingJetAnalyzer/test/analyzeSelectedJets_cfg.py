import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

########################################
# VarParsing begin
#
# Command line argument parsing
import FWCore.ParameterSet.VarParsing as VarParsing
# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')
options.register ('crab',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to run on CRAB.")
# options.crab = 0
options.register ('data',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 for data.")
# options.data = 0
sampleTypes = ['signal', 'background', 'wjet']
options.register ('sample',
                  'signal', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Specify type of sample. Valid values: " % sampleTypes)
# parse arguments
options.parseArguments()
if options.sample not in sampleTypes:
    print 'Invalid sample type. Setting to sample type to signal.'
    options.sample = 'signal'
#
# VarParsing end
########################################


process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_0T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
if options.data:
    process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v3', '')
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9', '')

from TrackingTools.TrackAssociator.default_cfi import *

# Other statements

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    # eventsToProcess = cms.untracked.VEventRange("1:36:3523-1:36:3523"),
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        # signal
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM/150717_090102/0000/aodsim_1.root'
        # wjet
        # '/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_1.root'
    ),
)

process.jetFilter = cms.EDFilter("JetFilter",
    srcJets = cms.InputTag("ak4PFJetsCHS"),
    # srcJets = cms.InputTag("patJets"),
    # additionalCut = cms.string(""),
    additionalCut = cms.string("abs(eta) < 2.5 && pt > 50.0"),
    jetCuts = cms.VPSet(
        cms.PSet(
            minPt = cms.double(400.0),
            maxEta = cms.double(2.5),
            stringCut = cms.string(""),
            ),
        cms.PSet(
            minPt = cms.double(200.0),
            maxEta = cms.double(2.5),
            stringCut = cms.string(""),
            ),
        cms.PSet(
            minPt = cms.double(125.0),
            maxEta = cms.double(2.5),
            stringCut = cms.string(""),
            ),
        cms.PSet(
            minPt = cms.double(50.0),
            maxEta = cms.double(2.5),
            stringCut = cms.string(""),
            ),
    )
)


process.emergingJetAnalyzer = cms.EDAnalyzer('EmergingJetAnalyzer',
    TrackAssociatorParameterBlock,
    srcJets = cms.InputTag("jetFilter", "selectedJets"),
    isData = cms.untracked.bool(False),
)
if options.sample == 'wjet':
    process.emergingJetAnalyzer.srcJets = cms.InputTag("wJetFilter")
if options.data:
    process.emergingJetAnalyzer.isData = cms.untracked.bool(True)

process.TFileService = cms.Service("TFileService", fileName = cms.string("output.root") )

process.p = cms.Path(process.jetFilter*process.emergingJetAnalyzer)
if options.sample == 'wjet':
    process.p = cms.Path(process.emergingJetAnalyzer)

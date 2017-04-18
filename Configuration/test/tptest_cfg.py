import FWCore.ParameterSet.Config as cms

########################################
# Command line argument parsing
########################################
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.register ('crab',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to run on CRAB.")
options.register ('data',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 for data.")
sample_options = ['signal', 'background', 'wjet'] # Valid options for sample
options.register ('sample',
                  'signal', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Specify type of sample. Valid values: %s" % sample_options)
steps_options = ['skim', 'analyze'] # Valid options for steps
options.register ('steps',
                  [],
                  VarParsing.VarParsing.multiplicity.list, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Steps to execute. Possible values: skim, analyze.")
options.steps = ['skim', 'analyze'] # default value
# Get and parse the command line arguments
options.parseArguments()
# Check validity of command line arguments
if options.sample not in sample_options:
    print 'Invalid sample type. Setting to sample type to signal.'
    options.sample = 'signal'
for step in options.steps:
    if step not in steps_options:
        print "Skipping invalid steps: %s" % step
        options.steps.remove(step)

print options.steps

from EmergingJetAnalysis.Configuration.emjetTools import *

process = cms.Process('ANALYSIS')

########################################
# Stable configuration
########################################
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

# Unscheduled execution
# process.options.allowUnscheduled = cms.untracked.bool(False)
process.options.allowUnscheduled = cms.untracked.bool(True)

########################################
# Skim
########################################
import os
cmssw_version = os.environ['CMSSW_VERSION']
skimStep = cms.Sequence()
if 'skim' in options.steps:
    print ''
    print '####################'
    print 'Adding Skim step'
    print '####################'
    print ''
    if options.sample=='wjet':
        skimStep = addWJetSkim(process, options.data)
        if 'CMSSW_7_4_12' in cmssw_version:
            process.wJetFilter.electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-medium')
        elif 'CMSSW_7_4_1_patch4' in cmssw_version:
            process.wJetFilter.electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium')
    else:
        skimStep = addSkim(process, options.data)
########################################
# Analyze
########################################
analyzeStep = cms.Sequence()
if 'analyze' in options.steps:
    print ''
    print '####################'
    print 'Adding Analyze step'
    print '####################'
    print ''
    analyzeStep = addAnalyze(process, options.data, options.sample)

process.p = cms.Path( skimStep * analyzeStep )

if 'skim' in options.steps and len(options.steps)==1:
    # If only running skim, add AOD/AODSIM and jetFilter/wJetFilter to output
    print ''
    print '####################'
    print 'Adding EDM output'
    print '####################'
    print ''
    addEdmOutput(process, options.data, options.sample)
else:
    # Otherwise only save EDM output of jetFilter and wJetFilter
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('output.root'),
        outputCommands = cms.untracked.vstring('drop *'),
    )
    if options.sample=='wjet' : process.out.outputCommands.extend(cms.untracked.vstring('keep *_wJetFilter_*_*',))
    else                      : process.out.outputCommands.extend(cms.untracked.vstring('keep *_jetFilter_*_*',))

########################################
# Tracking Particle test
########################################
# Copied from https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideTrackMCTruth

# Run TrackingParticle-Track association
process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
# process.load("SimTracker.TrackAssociatorProducers.trackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("EmergingJetAnalysis.TrackTruthAnalyzer.trackTruthAnalyzer_cfi")

# process.quickTrackAssociatorByHits.Quality_SimToReco = cms.double(0.25)
# process.quickTrackAssociatorByHits.Purity_SimToReco = cms.double(0.25)

# trackingParticleStep = cms.Sequence(process.trackingParticleRecoTrackAsssociation )
# process.trackingParticleRecoTrackAsssociation.associator = cms.InputTag('trackAssociatorByChi2')
trackingParticleStep = cms.Sequence(process.trackingParticleRecoTrackAsssociation * process.trackTruthAnalyzer)
process.p.insert(10000, trackingParticleStep)
process.out.outputCommands.extend(cms.untracked.vstring('keep *_trackingParticleRecoTrackAsssociation_*_*',))


########################################
# Generic configuration
########################################
if 'CMSSW_7_4_12' in cmssw_version:
    globalTags=['74X_mcRun2_design_v2','74X_dataRun2_Prompt_v3']
elif 'CMSSW_7_4_1_patch4' in cmssw_version:
    globalTags=['MCRUN2_74_V9','74X_dataRun2_Prompt_v0']
print 'CMSSW_VERSION is %s' % cmssw_version
print 'Using the following global tags [MC, DATA]:'
print globalTags
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTags[options.data], '')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.FwkReport.limit = 20
process.MessageLogger.cerr.default.limit = 100
process.MessageLogger.suppressWarning = cms.untracked.vstring('triggerSelection')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # eventsToProcess = cms.untracked.VEventRange("1:36:3523-1:36:3523"),
    fileNames = cms.untracked.vstring(
        # signal FEVTDEBUG ModelB
        'file:/afs/cern.ch/user/y/yoshin/CMSSW_7_4_1_patch4/src/emjet-gen/fevtdebugtest/ModelB/RECO_TrackingPart_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO.root'
        # signal
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM/150717_090102/0000/aodsim_1.root'
        # wjet
        # '/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_1.root'
        # wjet MC
        # '/store/mc/RunIISpring15DR74/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/006D71A7-73FC-E411-8C41-6CC2173BBE60.root'
    ),
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("ntuple.root") )

# storage
process.outpath = cms.EndPath(process.out)

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")


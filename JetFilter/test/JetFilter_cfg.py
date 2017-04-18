############################################################
# Important things to change before running
# - Default value for inputFiles
# - Global Tag (between releases)
# - datasets.py import location
############################################################

import FWCore.ParameterSet.Config as cms
# Command line argument parsing
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')


## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.Geometry.GeometryIdeal_cff")
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
options.inputFiles= '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150715_195547/0000/aodsim_10.root'
options.outputFile = ''
options.register ('crab',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to run on CRAB.")
options.crab = 0


# get and parse the command line arguments
options.parseArguments()


# # Unscheduled mode
# process.options.allowUnscheduled = cms.untracked.bool(False)
# process.options.allowUnscheduled = cms.untracked.bool(True)



### =========== Global configuration ==========================

## MET flavor configuration -> may be time consuming
## NoPu and MVA met need to be generated with some refeernce objects (e;g. leptons)
## look for the corresponding area in the config file to set your own definition


### ===========================================================
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.source = cms.Source("PoolSource",
		# fileNames = cms.untracked.vstring("file:input.root"),
    skipEvents = cms.untracked.uint32(0)
)
# if options.inputFiles :
process.source.fileNames = cms.untracked.vstring(options.inputFiles)

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
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    categories = cms.untracked.vstring('JetFilter'),
    debugModules = cms.untracked.vstring('jetFilter'),
    cout         = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        # DEBUG = cms.untracked.int32(0),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
        ),
    )
)
# process.MessageLogger.cout.INFO.limit = 0
# process.MessageLogger.cerr.threshold = "DEBUG"
# process.MessageLogger.categories.append('JetFilter')

# process.MessageLogger.cerr.FwkReport.reportEvery = 100
# process.MessageLogger.cerr.default.limit = 100
#process.options.wantSummary = False

############################################################
# PAT Default Configuration
############################################################
# process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
# process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
# process.load("PhysicsTools.PatAlgos.cleaningLayer1.cleanPatCandidates_cff")
# process.load("PhysicsTools.PatAlgos.selectionLayer1.countPatCandidates_cff")

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning

############################################################
# Private modules
############################################################
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
process.p = cms.Path( process.jetFilter )



############################################################
# PATH definition
############################################################
# process.muonPath = cms.Path( process.countPatMuons )
# process.electronPath = cms.Path( process.countPatElectrons )

# process.jetPath = cms.Path( process.jetFilter )




process.out = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    fileName = cms.untracked.string('output.root'),
		SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    #     # SelectEvents = cms.vstring('muonPath', 'electronPath')
    #     # SelectEvents = cms.vstring('muonPath')
		)
)
if options.outputFile=='.root':
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

process.out.outputCommands = cms.untracked.vstring(
		# 'keep *',
	'drop *',
	# 'keep *_generator_*_*',
  # 'keep edmTriggerResults_*_*_*',
	# 'keep *_pfMet_*_*',
	# 'keep *_zJetFilter_*_*',
	# *patEventContentNoCleaning
  'keep *_jetFilter_*_*',
)


# storage
process.outpath = cms.EndPath(process.out) #dummy



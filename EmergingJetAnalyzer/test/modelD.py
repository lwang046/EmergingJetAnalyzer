import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")



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
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_FULL', '')

from TrackingTools.TrackAssociator.default_cfi import *

# Other statements

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_1.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_10.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_2.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_3.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_4.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_5.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_6.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_7.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_8.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelD_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150525_131119/0000/step2_9.root'
    )
)

process.demo = cms.EDAnalyzer('EmergingJetAnalyzer',
        TrackAssociatorParameterBlock
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo_D.root") )

process.p = cms.Path(process.demo)

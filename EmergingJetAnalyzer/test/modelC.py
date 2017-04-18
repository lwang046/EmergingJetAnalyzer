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
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_1.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_10.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_11.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_13.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_15.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_17.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_18.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_19.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_2.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_20.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_21.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_22.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_26.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_28.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_29.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_3.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_31.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_32.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_33.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_34.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_36.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_37.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_38.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_4.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_40.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_41.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_42.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_43.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_46.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_47.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_48.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_5.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_54.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_57.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_58.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_59.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_6.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_60.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_61.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_63.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_64.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_65.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_68.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_7.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_71.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_72.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_73.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_75.root',
        '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_76.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_78.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_8.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_80.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_81.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_82.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_83.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_88.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_89.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_9.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_90.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_91.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_97.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_98.root',
    '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelC_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150523_235952/0000/step2_99.root'
    )
)

process.demo = cms.EDAnalyzer('EmergingJetAnalyzer',
        TrackAssociatorParameterBlock
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo_C.root") )

process.p = cms.Path(process.demo)

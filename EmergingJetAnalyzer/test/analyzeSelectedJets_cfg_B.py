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
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

process.source = cms.Source("PoolSource",
    # eventsToProcess = cms.untracked.VEventRange("1:36:3523-1:36:3523"),
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_1.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_10.root',
#'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_100.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_11.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_12.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_13.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_14.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_15.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_16.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_17.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_18.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_19.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_2.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_20.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_21.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_22.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_23.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_24.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_25.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_26.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_27.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_28.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_29.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_3.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_30.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_31.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_32.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_33.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_34.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_35.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_36.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_37.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_38.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_39.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_4.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_40.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_41.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_42.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_43.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_44.root',
#'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_45.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_46.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_47.root',
#'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_48.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_49.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_5.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_50.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_51.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_52.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_53.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_54.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_55.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_56.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_57.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_58.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_59.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_6.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_60.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_61.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_62.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_63.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_64.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_65.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_66.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_67.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_68.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_69.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_7.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_70.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_71.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_72.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_73.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_74.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_75.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_76.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_77.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_78.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_79.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_8.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_80.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_81.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_82.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_83.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_84.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_85.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_86.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_87.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_88.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_89.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_9.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_90.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_91.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_92.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_93.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_94.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_95.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_96.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_97.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_98.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191640/0000/EmergingJets_ModelB_RECO_99.root'
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
    isData = cms.untracked.bool(True),
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("output.root") )

process.p = cms.Path(process.jetFilter*process.emergingJetAnalyzer)

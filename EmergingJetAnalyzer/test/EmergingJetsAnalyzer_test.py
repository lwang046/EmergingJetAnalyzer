import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")


process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_FULL', '')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_1.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_10.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_100.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_11.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_12.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_13.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_14.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_15.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_16.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_17.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_18.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_19.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_2.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_20.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_21.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_22.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_23.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_24.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_25.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_26.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_27.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_28.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_29.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_3.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_30.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_31.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_32.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_33.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_34.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_35.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_36.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_37.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_38.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_39.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_4.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_40.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_41.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_42.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_43.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_44.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_45.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_46.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_47.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_48.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_49.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_5.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_50.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_51.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_52.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_53.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_54.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_55.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_56.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_57.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_58.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_59.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_6.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_60.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_61.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_62.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_63.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_64.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_65.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_66.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_67.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_68.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_69.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_7.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_70.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_71.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_72.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_73.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_74.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_75.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_76.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_77.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_78.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_79.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_8.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_80.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_81.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_82.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_83.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_84.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_85.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_86.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_87.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_88.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_89.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_9.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_90.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_91.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_92.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_93.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_94.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_95.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_96.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_97.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_98.root',
'/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/RECO/150511_191618/0000/EmergingJets_ModelA_RECO_99.root'
    )
)

process.demo = cms.EDAnalyzer('EmergingJetAnalyzer'
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") )

process.p = cms.Path(process.demo)

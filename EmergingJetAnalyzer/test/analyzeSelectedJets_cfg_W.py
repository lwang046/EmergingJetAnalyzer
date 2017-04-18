import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")


process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Express_v2', '')

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
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_1.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_10.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_100.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_101.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_102.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_103.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_104.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_105.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_106.root',
'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_107.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_108.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_109.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_11.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_110.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_111.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_112.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_113.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_114.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_115.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_116.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_117.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_118.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_119.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_12.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_120.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_121.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_122.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_123.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_124.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_125.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_126.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_127.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_128.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_129.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_13.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_130.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_131.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_132.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_133.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_134.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_135.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_136.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_137.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_138.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_14.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_140.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_141.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_142.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_143.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_144.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_145.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_146.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_147.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_148.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_149.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_15.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_150.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_151.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_152.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_153.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_154.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_155.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_156.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_157.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_158.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_159.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_16.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_160.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_161.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_162.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_163.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_164.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_165.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_166.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_167.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_168.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_169.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_17.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_170.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_171.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_172.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_173.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_174.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_175.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_176.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_177.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_178.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_179.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_18.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_180.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_181.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_182.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_183.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_184.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_185.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_186.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_187.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_188.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_189.root',
# 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_19.root',
    ),
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = 'Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt').getVLuminosityBlockRange()


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
#             ),
#         cms.PSet(
#             minPt = cms.double(200.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#             ),
#         cms.PSet(
#             minPt = cms.double(125.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#             ),
#         cms.PSet(
#             minPt = cms.double(50.0),
#             maxEta = cms.double(2.5),
#             stringCut = cms.string(""),
#             ),
#     )
# )


process.emergingJetAnalyzer = cms.EDAnalyzer('EmergingJetAnalyzer',
    TrackAssociatorParameterBlock,
    srcJets = cms.InputTag("ak4PFJetsCHS"),
    isData = cms.untracked.bool(True),
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("output_W.root") )

# process.p = cms.Path(process.jetFilter*process.emergingJetAnalyzer)
process.p = cms.Path(process.emergingJetAnalyzer)

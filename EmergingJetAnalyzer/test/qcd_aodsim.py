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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_7_3_3/RelValQCD_Pt_600_800_13/GEN-SIM-RECO/MCRUN2_73_V11-v1/00000/02C2A99A-CAC3-E411-9804-002618943836.root',
        '/store/relval/CMSSW_7_3_3/RelValQCD_Pt_600_800_13/GEN-SIM-RECO/MCRUN2_73_V11-v1/00000/1C6DA8E9-C1C3-E411-A9EE-0025905A4964.root',
        '/store/relval/CMSSW_7_3_3/RelValQCD_Pt_600_800_13/GEN-SIM-RECO/MCRUN2_73_V11-v1/00000/3AF349D6-D0C3-E411-B6EC-002618943916.root',
        '/store/relval/CMSSW_7_3_3/RelValQCD_Pt_600_800_13/GEN-SIM-RECO/MCRUN2_73_V11-v1/00000/BA90CBD7-D0C3-E411-A649-002618943948.root'
    )
)

process.demo = cms.EDAnalyzer('EmergingJetAnalyzer'
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo_qcd_relval.root") )

process.p = cms.Path(process.demo)

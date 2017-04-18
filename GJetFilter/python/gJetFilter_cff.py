import FWCore.ParameterSet.Config as cms

process.gJetFilter = cms.EDFilter("GJetFilter",
    isData = cms.bool( False ),
    srcPhotons = cms.InputTag("slimmedPhotons"),
    srcJets = cms.InputTag("slimmedJets"),
    minPtPhoton = cms.double(175.0),
    minDeltaR = cms.double(0.4),
    maxDeltaPhi = cms.double(0.4), # Doesn't do anything
    minPtSelectedJet = cms.double(20.0),#
    maxPtAdditionalJets = cms.double(20.0), # Doesn't do anything
    #photonID = cms.string('cutBasedPhotonID-Spring15-25ns-V1-standalone-medium'),
    photonID = cms.string('cutBasedPhotonID-Spring15-50ns-V1-standalone-medium'),
)

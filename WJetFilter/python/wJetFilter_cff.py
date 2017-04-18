import FWCore.ParameterSet.Config as cms

process.wJetFilter = cms.EDFilter("WJetFilter",
    isData = cms.bool( False ),
    srcMuons = cms.InputTag("slimmedMuons"),
    srcElectrons = cms.InputTag("slimmedElectrons"),
    srcJets = cms.InputTag("slimmedJets"),
    srcMET = cms.InputTag("slimmedMETs"),
    minPtMuon = cms.double(20.0),
    minPtElectron = cms.double(20.0),
    minPtMET = cms.double(20.0),
    minMt = cms.double(50.0),
    maxMt = cms.double(100.0),
    minDeltaR = cms.double(0.4),
    maxDeltaPhi = cms.double(0.4), # Doesn't do anything
    minPtSelectedJet = cms.double(20.0),
    maxPtAdditionalJets = cms.double(20.0), # Doesn't do anything
    electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-loose'),
    # electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium'),
)

import FWCore.ParameterSet.Config as cms

jetFilter = cms.EDFilter("JetFilter",
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

import FWCore.ParameterSet.Config as cms

emJetAnalyzer = cms.EDFilter('EmJetAnalyzer',
    TrackAssociatorParameterBlock,
    srcJets = cms.InputTag("jetFilter", "selectedJets"),
    isData = cms.bool(False),
    vertexreco = cms.PSet(
        primcut = cms.double( 3.0 ),
        seccut = cms.double( 5.0 ),
        smoothing = cms.bool( True ),
        # weightthreshold = cms.double( 0.0010 ),
        minweight = cms.double( 0.5 ),
        finder = cms.string( "avr" )
    ),
    # scanJet = cms.bool(False),
    # scanJet = cms.
)

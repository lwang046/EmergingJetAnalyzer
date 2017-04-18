import FWCore.ParameterSet.Config as cms

trackTruthAnalyzer = cms.EDFilter('TrackTruthAnalyzer',
                                  tracksTag = cms.InputTag('generalTracks'),
                                  tpTag = cms.InputTag('mix','MergedTrackTruth'),
                                  recSim = cms.InputTag('trackingParticleRecoTrackAsssociation'),
                                  simRec = cms.InputTag('trackingParticleRecoTrackAsssociation'),
)

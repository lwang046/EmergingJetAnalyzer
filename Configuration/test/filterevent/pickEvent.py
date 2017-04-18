import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
# add a list of strings for events to process
options.register ('eventsToProcess',
                                  '',
                                  VarParsing.multiplicity.list,
                                  VarParsing.varType.string,
                                  "Events to process")
options.parseArguments()

process = cms.Process("PickEvent")
process.source = cms.Source ("PoolSource",
          fileNames = cms.untracked.vstring (
             '/store/mc/RunIIFall15DR76/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/2E1C97FD-AD9F-E511-A2B0-02163E013DFA.root'
          ),
          eventsToProcess = cms.untracked.VEventRange (
          #   options.eventsToProcess
              '1:8693:15872617',
          )                               
)

process.Out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string ('output.root')
)

process.end = cms.EndPath(process.Out)

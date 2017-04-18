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

#set up a process , for e.g. POSTSKIM in this case
processName = "POSTSKIM"
process = cms.Process(processName)



# this inputs the input files
process.source = cms.Source ("PoolSource",
          fileNames = cms.untracked.vstring (options.inputFiles),
          eventsToProcess = cms.untracked.VEventRange (options.eventsToProcess)
)


# loads your analyzer
process.pfjetToTrack = cms.EDProducer('PFJetTrackAnalyzer',
    srcJets = cms.InputTag("ak4PFJetsCHS"),
)

# talk to output module
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string (options.outputFile),
    outputCommands = cms.untracked.vstring(
        'keep *',
    )
)

# Defines which modules and sequences to run
process.mypath = cms.Path(process.pfjetToTrack)

# A list of analyzers or output modules to be run after all paths have been run.
process.outpath = cms.EndPath(process.out)

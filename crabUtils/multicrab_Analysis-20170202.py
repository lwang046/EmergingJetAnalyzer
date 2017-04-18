"""MultiCRAB script template for EmergingJet analysis"""
MC=0
DATA=1

jobname = 'Analysis-20170202'        # Jobname
psetname = 'Configuration/test/test_cfg.py'      # Path to pset
postfix = 'v0'                # Postfix to job, increment for each major version
dryrun = 0

from datasets import dataset
from wrappers import submit, submit_newthread
datasets = [
    dataset( "WJetsToLNuInclusive" , "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='wjet' ),
    # dataset( "Dummy"                 , "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"       , MC   , 1    , 1        , splitting='FileBased'           , priority=99 , label='wjet' ) ,
    # dataset( "ModelA"                , "/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/yoshin-AODSIM-69070e02f00a6fbca8a74a9d93177037/USER"                        , MC   , 1     , 10000000 , splitting='FileBased'           , priority=99 , label='signal'   , inputDBS='phys03' ) ,
    # dataset( "ModelB"                , "/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/yoshin-AODSIM-69070e02f00a6fbca8a74a9d93177037/USER"                        , MC   , 1     , 10000000 , splitting='FileBased'           , priority=99 , label='signal'   , inputDBS='phys03' ) ,
    # dataset( "QCD_HT500to700"        , "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"                    , MC   , 1     , 100      , splitting='FileBased'           , priority=10 , label='signal' ) ,
    # dataset( "QCD_HT700to1000"       , "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"                   , MC   , 1     , 1000     , splitting='FileBased'           , priority=15 , label='signal' ) ,
    # dataset( "QCD_HT1000to1500"      , "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM"                  , MC   , 1     , 100      , splitting='FileBased'           , priority=30 , label='signal' ) ,
    # dataset( "QCD_HT1500to2000"      , "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"                  , MC   , 1     , 100      , splitting='FileBased'           , priority=28 , label='signal' ) ,
    # dataset( "QCD_HT2000toInf"       , "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM"                   , MC   , 1     , 100      , splitting='FileBased'           , priority=25 , label='signal' ) ,
    # dataset( "QCD_HT500to700_76X"    , "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM"   , MC   , 1     , 100      , splitting='FileBased'           , priority=10 , label='signal' ) ,
    # dataset( "QCD_HT700to1000_76X"   , "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM"  , MC   , 1     , 1000     , splitting='FileBased'           , priority=15 , label='signal' ) ,
    # dataset( "QCD_HT1000to1500_76X"  , "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM" , MC   , 1     , 100      , splitting='FileBased'           , priority=30 , label='signal' ) ,
    # dataset( "QCD_HT1500to2000_76X"  , "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM" , MC   , 1     , 100      , splitting='FileBased'           , priority=28 , label='signal' ) ,
    # dataset( "QCD_HT2000toInf_76X"   , "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM"  , MC   , 1     , 100      , splitting='FileBased'           , priority=25 , label='signal' ) ,
    # dataset( "DataSkim_Run2015-PRv3" , "/JetHT/yoshin-DataSkim-20160302-80c51f15cd036b2256e94c207509265d/USER"                                                         , DATA , 1000  , 20000    , splitting='EventAwareLumiBased' , priority=35 , label='signal'   , inputDBS='phys03' ) ,
    # dataset( "WJetSkimMuon"          , "/SingleMuon/yoshin-WJetSkim-ede3f21fae18a825b193df32c86b780e/USER"                                                             , DATA , 10000 , 10000000 , splitting='EventAwareLumiBased' , priority=30 , label='wjet'     , inputDBS='phys03' ) ,
]

if __name__ == '__main__':

    ############################################################
    ## Common settings
    ############################################################
    from WMCore.Configuration import Configuration
    import time
    config = Configuration()

    config.section_("General")
    config.General.workArea = 'crabTasks/' + 'crab_' + jobname + time.strftime("-%Y-%m%d") + '-' + postfix
    tasklistFileName = config.General.workArea + '.txt'
    if not dryrun: tasklistFile = open(tasklistFileName, 'a')

    config.section_("JobType")
    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = psetname
    # config.JobType.scriptExe = 'crab_script.sh'                                # Script to run instead of cmsRun
    # config.JobType.inputFiles = ['emjet-basicJetAnalyzer.py','crab_script.py'] # Additional input files
    # config.JobType.outputFiles = ['histo.root']                                # Collect non-EDM outputs if any

    config.section_("Data")
    config.Data.splitting = 'FileBased'
    config.Data.publication = True
    config.Data.outputDatasetTag = jobname
    config.Data.ignoreLocality = True

    config.section_("Site")
    config.Site.storageSite = "T3_US_UMD"
    config.Site.blacklist = ['T2_US_Purdue']
    # config.Site.whitelist = ['T2_CH_CERN']
    # config.Site.ignoreGlobalBlacklist = True

    ############################################################
    ## Dataset specific settings
    ############################################################
    for dataset in datasets:
        alias = dataset.alias
        pyCfgParams = ['crab=1'] # Temporary list, must be written to config.JobType.pyCfgParams before submitting
        config.General.requestName   = ('%s-%s' + time.strftime("-%Y-%m%d-%H%M%S")) % (jobname, alias)
        config.Data.outLFNDirBase = '/store/group/EmJet/ntuple/%s-%s/%s/' % (jobname, postfix, alias)
        config.Data.inputDataset = dataset.fullpath
        config.Data.unitsPerJob  = dataset.unitsPerJob
        config.Data.totalUnits   = dataset.totalUnits
        isData                   = dataset.isData
        config.Data.splitting    = dataset.splitting
        config.JobType.priority  = dataset.priority
        config.Data.inputDBS     = dataset.inputDBS
        label                    = dataset.label
        # MC specific settings:
        if not isData:
            pyCfgParams.append('data=0')
        # Data specific settings:
        if isData:
            pyCfgParams.append('data=1')
            config.Data.lumiMask = 'http://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
            pyCfgParams.append('steps=analyze') # Only run analyze step on data (Applicable if datasets has been skimmed already)
        # Label specific settings
        pyCfgParams.append('sample='+label)
        # Use this turn of different steps
        # pyCfgParams.append('steps=skim')
        # pyCfgParams.append('steps=analyze')
        config.JobType.pyCfgParams = pyCfgParams

        if not dryrun:
            res = submit_newthread(config, dryrun=dryrun)
            taskId = res['uniquerequestname'].split(':')[0]
            filePath = '%s%s/%s/%s/' % ( config.Data.outLFNDirBase, config.Data.inputDataset.split('/')[1], config.Data.outputDatasetTag, taskId )
            print 'filePath:'
            print filePath
            tasklistFile.write(filePath)
            tasklistFile.write('\n')

    if not dryrun: tasklistFile.close()


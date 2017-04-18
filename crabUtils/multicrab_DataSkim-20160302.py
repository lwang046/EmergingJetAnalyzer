"""MultiCRAB script template for EmergingJet analysis"""
MC=0
DATA=1

jobname = 'DataSkim-20160302'        # Jobname
psetname = 'test_cfg.py'      # Path to pset
postfix = 'v0'                # Postfix to job, increment for each major version
dryrun = False

from datasets import dataset
from wrappers import submit, submit_newthread
datasets = [
    dataset( "Run2015D" , "/JetHT/Run2015D-PromptReco-v3/AOD" , DATA, 100000, 80000000, splitting='EventAwareLumiBased', priority=10, label='signal' ),
]

if __name__ == '__main__':

    ############################################################
    ## Common settings
    ############################################################
    from WMCore.Configuration import Configuration
    import time
    config = Configuration()

    config.section_("General")
    config.General.workArea = 'crab_' + jobname + time.strftime("-%Y-%m%d") + '-' + postfix
    tasklistFileName = config.General.workArea + '.txt'
    if not dryrun: tasklistFile = open(tasklistFileName, 'a')

    config.section_("JobType")
    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = psetname
    config.JobType.pyCfgParams = ['crab=1'] # Must be overwritten for data
    # config.JobType.scriptExe = 'crab_script.sh'                                # Script to run instead of cmsRun
    # config.JobType.inputFiles = ['emjet-basicJetAnalyzer.py','crab_script.py'] # Additional input files
    # config.JobType.outputFiles = ['histo.root']                                # Collect non-EDM outputs if any

    config.section_("Data")
    config.Data.splitting = 'FileBased'
    config.Data.publication = True
    config.Data.outputDatasetTag = jobname

    config.section_("Site")
    config.Site.storageSite = "T2_CH_CERN"
    config.Site.blacklist = ['T2_US_Purdue']

    ############################################################
    ## Dataset specific settings
    ############################################################
    for dataset in datasets:
        alias = dataset.alias
        config.General.requestName   = ('%s-%s' + time.strftime("-%Y-%m%d-%H%M%S")) % (jobname, alias)
        config.Data.outLFNDirBase = '/store/group/phys_exotica/EmergingJets/%s-%s/%s/' % (jobname, postfix, alias)
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
            config.JobType.pyCfgParams.append('data=0')
        # Data specific settings:
        if isData:
            config.JobType.pyCfgParams.append('data=1')
            config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
        # Label specific settings
        config.JobType.pyCfgParams.append('sample='+label)
        # Use this turn on different steps
        config.JobType.pyCfgParams.append('steps=skim')
        # config.JobType.pyCfgParams.append('steps=analyze')

        if not dryrun:
            res = submit_newthread(config)
            taskId = res['uniquerequestname'].split(':')[0]
            filePath = '%s%s/%s/%s/' % ( config.Data.outLFNDirBase, config.Data.inputDataset.split('/')[1], config.Data.outputDatasetTag, taskId )
            print 'filePath:'
            print filePath
            tasklistFile.write(filePath)
            tasklistFile.write('\n')

    if not dryrun: tasklistFile.close()


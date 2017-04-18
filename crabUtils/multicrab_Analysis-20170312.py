"""MultiCRAB script template for EmergingJet analysis"""
MC=0
DATA=1

# this is for another set of Gamma +Jet MC samples. Photon Pt cut has been changed to 175GeV for 
#   fake rate check(for jobs on 0308(v4), the Photon pt cut is 50GeV)

jobname = 'Analysis-20170312'        # Jobname
psetname = 'Configuration/test/test_cfg.py'      # Path to pset
postfix = 'v5'                # Postfix to job, increment for each major version
dryrun = 0

from datasets import dataset
from wrappers import submit, submit_newthread
datasets = [
    #dataset( "WJetsToLNuInclusive" , "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='wjet' ),
    # dataset( "WJetSkimMuon"          , "/SingleMuon/yoshin-WJetSkim-ede3f21fae18a825b193df32c86b780e/USER"                                                             , DATA , 10000 , 10000000 , splitting='EventAwareLumiBased' , priority=30 , label='wjet'     , inputDBS='phys03' ) ,
    #dataset( "GJet_Pt-15ToInf_PhotonPtcut50" , "/GJet_Pt-15ToInf_TuneCUETP8M1_13TeV-pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ), 
    dataset( "GJet_Pt-15To6000-Flat_PhotonPtcut175" , "/GJet_Pt-15To6000_TuneCUETP8M1-Flat_13TeV_pythia8/RunIIFall15DR76-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJetDataRun2015D-v3" , "/SinglePhoton/Run2015D-PromptReco-v3/AOD" , DATA, 1, 10000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJetDataRun2015D-v4" , "/SinglePhoton/Run2015D-PromptReco-v4/AOD" , DATA, 1, 10000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJets_HT-600ToInf_RunIISpring15DR74" , "/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJets_HT-400To600_RunIISpring15DR74" , "/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJets_HT-200To400_RunIISpring15DR74" , "/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJets_HT-100To200_RunIISpring15DR74" , "/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
    #dataset( "GJets_HT-40To100_RunIISpring15DR74" , "/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM" , MC, 1, 1000, splitting='FileBased', priority=30, label='gjet' ),
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
    if not dryrun: tasklistFile = open(tasklistFileName, 'a+')

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
        config.Data.outLFNDirBase = '/store/user/yofeng/GJet/ntuple/%s-%s/%s/' % (jobname, postfix, alias)
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
            #pyCfgParams.append('steps=analyze') # Only run analyze step on data (Applicable if datasets has been skimmed already)
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


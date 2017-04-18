"""Wrapper functions for CRABAPI commands"""
from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException
from multiprocessing import Process, Queue

def submit(config, q):
    try:
        res = crabCommand('submit', config = config)
        q.put(res)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

def submit_newthread(config, dryrun=False):
    # Works around modification of config when running over multiple datasets
    # See https://hypernews.cern.ch/HyperNews/CMS/get/computing-tools/622/1/1/2/1/1/1/1.html
    q = Queue()
    if dryrun:
        p = Process(target=submit_dryrun, args=(config, q))
    else:
        p = Process(target=submit, args=(config, q))
    p.start()
    p.join()
    res = q.get()
    return res

def submit_dryrun(config, q):
    print 'submit_dryrun'
    try:
        res = crabCommand('submit', '--dryrun', '--skip-estimates', config = config)
        q.put(res)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

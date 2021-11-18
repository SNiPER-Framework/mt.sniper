#!/usr/bin/env python

def HelloJob():
    # configuration is just the same as a single thread job
    import Sniper
    global ith
    ith += 1
    task = Sniper.Task(str(ith) +"-Job")

    bufMgr = task.createSvc("MtBufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.1, 0.1])

    iSvc = task.createSvc("ThrdInputSvc/InputSvc")
    x = task.createAlg("DummyAnalyAlg")

    global first_time
    if first_time:
        task.show()
        first_time = False
    # instead of task.run(), we must return the task object here
    return task


def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.1, 0.1]);

    import RootIOSvc
    riSvc = task.createSvc("RootInputSvc/InputSvc")
    riSvc.property("InputFile").set(["/sharefs/bes/zoujh/juno/MtAna/mt.sniper/0.5M_OEC.root"])

    x = task.createAlg("PackFragAlg")
    x.property("MaxFragSize").set(5)

    return task

def GOutput():
    import Sniper
    task = Sniper.Task("GOutput")
    task.createAlg("ClearGBufferAlg")

    return task

if __name__ == "__main__":

    import ROOT
    ROOT.EnableThreadSafety()

    import Sniper
    import SniperMuster
    import CoAnalysis

    global ith
    ith = 0
    global first_time
    first_time = True

    # we will execute the HelloJob maximumly in 4 threads
    muster = SniperMuster.Muster()
    muster.config(HelloJob, 4)

    # I/O and global buffer
    gs = SniperMuster.createGlobalStream("FragmentStream/GFragStream")
    gs.configInput(GInput)
    gs.configOutput(GOutput)
    gs.configBuffer(50, 20)

    # TODO: show the configurations
    #muster.show()

    # the log level and EvtMax in Sniper.Muster
    Sniper.setLogLevel(3)
    muster.setEvtMax(10000)

    # spawn the threads and begin to run 
    muster.run()

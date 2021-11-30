#!/usr/bin/env python

def HelloJob():
    import SniperMuster
    task = SniperMuster.createWorker("Task/Job")

    # configuration is just the same as a single thread job
    import MtIOExample
    x = task.createAlg("DummyIOAlg/x")

    mMgr = task.createSvc("DummyMemMgr")
    iSvc = task.createSvc("DummyInputSvc/InputSvc")
    oSvc = task.createSvc("DummyOutputSvc/OutputSvc")

    # instead of task.run(), we must return the task object here
    return task

def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    fa = task.createAlg("FillGBufAlg")
    fa.property("GenMax").set(-1)

    return task

def GOutput():
    import Sniper
    task = Sniper.Task("GOutput")
    wa = task.createAlg("WriteGBufAlg")
    wa.property("DataFile").set("data.file")

    return task

if __name__ == "__main__":

    import Sniper
    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    import SniperMuster
    muster = SniperMuster.Muster()

    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(100000)

    # we will execute the HelloJob maximumly in 4 threads
    muster.config(HelloJob, 4)

    # I/O and global buffer
    gs = SniperMuster.createGlobalStream("DummyEventStream/GEvtStream")
    gs.configBuffer(50, 20)  #the buffer must be created before the I/O tasks
    gs.configInput(GInput)
    gs.configOutput(GOutput)

    # show the configurations
    SniperMuster.show()

    # spawn the threads and begin to run 
    muster.run()
    gs.join()

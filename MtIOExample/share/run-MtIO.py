#!/usr/bin/env python

def HelloJob():
    task = SniperMuster.createWorker("Task/Job")

    # configuration is just the same as a single thread job
    mMgr = task.createSvc("DummyMemMgr")
    iSvc = task.createSvc("DummyInputSvc/InputSvc")
    oSvc = task.createSvc("DummyOutputSvc/OutputSvc")
    global root_writer
    task.addSvc(root_writer)

    task.createAlg("DummyIOAlg/x")
    task.createAlg("WriteRootAlg")

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
    import SniperMuster
    import RootWriter
    import SniperRootUsages
    import MtIOExample

    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    global root_writer
    root_writer = Sniper.create("MtRootWriter/RootWriter")
    root_writer.property("Output").set({"FILE1": "output1.root", "FILE2": "output2.root"})

    # I/O and global buffer
    gs = SniperMuster.createGlobalStream("GlobalStream4Any/GlobalStream")
    gs.configBuffer(50, 20)  #the buffer must be created before the I/O tasks
    gs.configInput(GInput)
    gs.configOutput(GOutput)

    # setup the Muster
    muster = SniperMuster.Muster()
    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(100000)

    # we will execute the HelloJob maximumly in 4 threads with the global stream
    muster.setIO(gs)
    muster.config(HelloJob, 4)

    # show the configurations
    SniperMuster.show()

    # spawn the threads and begin to run 
    muster.run()

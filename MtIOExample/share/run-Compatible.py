#!/usr/bin/env python

def HelloJob():
    task = SniperMuster.createWorker("Task/Job")

    #global root_writer
    #task.addSvc(root_writer)

    task.createSvc("OldStyleGetGlobalBufSvc/GetGlobalBufSvc")
    task.createSvc("SniperProfiling")
    alg = task.createAlg("TimeConsumeAlg")
    alg.createTool("TimeConsumeTool")

    # instead of task.run(), we must return the task object here
    return task

def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    task.createSvc("OldStyleFillGlobalBufSvc/FillGlobalBufSvc")
    task.createAlg("FillGlobalBufAlg")

    return task

def GOutput():
    import Sniper
    task = Sniper.Task("GOutput")
    task.createSvc("OldStyleGetGlobalBufSvc/GetGlobalBufSvc")
    wa = task.createAlg("PruneGlobalBufAlg")
    wa.property("OutputFile").set("mtresult.txt")

    return task

if __name__ == "__main__":

    import Sniper
    import SniperMuster
    import SniperProfiling
    #import RootWriter
    #import SniperRootUsages
    import SniperCoreUsages
    import MtIOExample

    nthrd = 8
    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    #global root_writer
    #root_writer = Sniper.create("MtRootWriter/RootWriter")
    #root_writer.property("Output").set({"FILE1": "output1.root", "FILE2": "output2.root"})

    # I/O and global buffer
    gs = SniperMuster.createGlobalStream("GlobalStream4Any/GlobalStream")
    gs.configBuffer(nthrd*3, nthrd+2)  #the buffer must be created before the I/O tasks
    gs.configInput(GInput)
    gs.configOutput(GOutput)

    # setup the Muster
    muster = SniperMuster.Muster()
    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(3000)

    # we will execute the HelloJob maximumly in 4 threads with the global stream
    muster.setIO(gs)
    muster.config(HelloJob, nthrd)

    # show the configurations
    #SniperMuster.show()

    # spawn the threads and begin to run 
    muster.run()

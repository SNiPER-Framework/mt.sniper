#!/usr/bin/env python

def HelloJob():
    # configuration is just the same as a single thread job
    import Sniper
    global ith
    ith += 1
    task = Sniper.Task(str(ith) +"-Job")

    import MtIOExample
    x = task.createAlg("DummyIOAlg/x")
    #x.property("VarString").set("GOD")

    mMgr = task.createSvc("DummyMemMgr")
    iSvc = task.createSvc("DummyInputSvc/InputSvc")
    oSvc = task.createSvc("DummyOutputSvc/OutputSvc")

    global first_time
    if first_time:
        task.show()
        first_time = False
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

    global ith
    ith = 0
    global first_time
    first_time = True

    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(100000)

    # we will execute the HelloJob maximumly in 4 threads
    muster.config(HelloJob, 4)

    # I/O and global buffer
    gs = SniperMuster.GlobalStream("GEvtStream")
    gs.configInput(GInput)
    gs.configOutput(GOutput)
    gs.configBuffer(50, 20)

    # TODO: show the configurations
    #muster.show()

    # spawn the threads and begin to run 
    muster.run()

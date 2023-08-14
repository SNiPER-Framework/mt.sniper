#!/usr/bin/env python

def HelloJob():
    task = SniperMuster.createWorker("Task/Job")
    task.createSvc("SniperProfiling")

    #############
    alg = task.createAlg("FanOutAlg/input")
    alg.property("InitKeys").set(["v1", "v2", "v4", "sum12", "sum24"])
    alg.property("outputs").set(["input"])

    alg = task.createAlg("TimeConsumeAlg/v1")
    alg.property("TimeScale").set(1)
    alg.property("inputs").set(["input"])
    alg.property("outputs").set(["v1"])
    alg.createTool("TimeConsumeTool")

    alg = task.createAlg("TimeConsumeAlg/v2")
    alg.property("TimeScale").set(2)
    alg.property("inputs").set(["input"])
    alg.property("outputs").set(["v2"])
    alg.createTool("TimeConsumeTool")

    alg = task.createAlg("TimeConsumeAlg/v4")
    alg.property("TimeScale").set(4)
    alg.property("inputs").set(["input"])
    alg.property("outputs").set(["v4"])
    alg.createTool("TimeConsumeTool")

    alg = task.createAlg("FanInAlg/sum12")
    alg.property("inputs").set(["v1", "v2"])
    alg.property("outputs").set(["sum12"])

    alg = task.createAlg("FanInAlg/sum24")
    alg.property("inputs").set(["v2", "v4"])
    alg.property("outputs").set(["sum24"])

    if useRootWriter:
        task.addSvc(root_writer)
        task.createSvc("EndEvtHandler4MtRootWriter")
        task.findAlg("input").createTool("FillRootTool/FillResultTool")
        task.findAlg("v1").createTool("FillRootTool/FillResultTool")
        task.findAlg("v2").createTool("FillRootTool/FillResultTool")
        task.findAlg("v4").createTool("FillRootTool/FillResultTool")
        task.findAlg("sum12").createTool("FillRootTool/FillResultTool")
        task.findAlg("sum24").createTool("FillRootTool/FillResultTool")

    # this service must be created after EndEvtHandler4MtRootWriter
    # there is an EndEvent handlers in each service and this one depends on another one
    task.createSvc("OldStyleGetGlobalBufSvc/GetGlobalBufSvc")

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
    if useRootWriter:
        tool = wa.createTool("WriteRootTool/WriteResultTool")
    else:
        tool = wa.createTool("WriteAsciiTool/WriteResultTool")
        tool.property("OutputFile").set("mtresult.txt")

    return task

if __name__ == "__main__":

    import Sniper
    import SniperMuster
    import SniperProfiling
    import SniperCoreUsages
    import MtIOExample

    useRootWriter = False
    nthrd = 4
    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    if useRootWriter:
        import RootWriter
        import SniperRootUsages
        root_writer = Sniper.create("MtRootWriter/RootWriter")
        root_writer.property("Output").set({"MtsTest": "mtresult.root"})

    # I/O and global buffer
    gs = SniperMuster.createGlobalStream("GlobalStream4Any/GlobalStream")
    gs.configBuffer(nthrd*3, nthrd*2)  #the buffer must be created before the I/O tasks
    gs.configInput(GInput)
    gs.configOutput(GOutput)

    # setup the Muster
    muster = SniperMuster.Muster()
    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(100)

    # we will execute the HelloJob maximumly in 4 threads with the global stream
    muster.setIO(gs)
    muster.config(HelloJob, nthrd)

    # show the configurations
    #SniperMuster.show()

    # spawn the threads and begin to run 
    muster.run()

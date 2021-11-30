#!/usr/bin/env python

def HelloJob():
    import SniperMuster
    task = SniperMuster.createWorker("Task")

    # configuration is just the same as a single thread job
    global svc
    task.addSvc(svc)

    alg = task.createAlg("SvcAndToolAlg")
    toolname = "tool1"
    alg.property("OwnedTool").set(toolname)
    tool = alg.createTool("DummyTool/"+toolname)
    tool.setDescription("a DummyTool instance owned by alg")
    tool.setLogLevel(4)

    # instead of task.run(), we must return the task object here
    return task


if __name__ == "__main__":

    import Sniper
    import SniperCoreUsages

    global svc
    svc = Sniper.create("SharedElem<DummySvc>/SharedSvc")
    svc.setDescription("a service instance that share by different Tasks")

    import SniperMuster
    muster = SniperMuster.Muster()

    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(20000)

    # we will execute the HelloJob maximumly in 4 threads
    muster.config(HelloJob, 4)

    # show the configurations
    SniperMuster.show()

    # spawn the threads and begin to run 
    muster.run()

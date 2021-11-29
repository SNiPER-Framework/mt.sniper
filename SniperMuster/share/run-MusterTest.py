#!/usr/bin/env python

def HelloJob():
    # configuration is just the same as a single thread job
    global ith
    ith += 1
    task = Sniper.Task(str(ith) +"-Job")
    task.setLogLevel(3)

    #task.property("algs").append("HelloWorld/x")
    #x = task.find("x")
    #x.property("VarBool").set(True)
    #x.property("VecFloat").set([0.01])
    #x.property("PairDVD").set([0.09, [1.1, 2.2, 3.3]])
    #x.property("MapIntStr").set({1: 'str1'})

    global svc
    task.addSvc(svc)

    alg = task.createAlg("SvcAndToolAlg")
    toolname = "tool1"
    alg.property("OwnedTool").set(toolname)
    tool = alg.createTool("DummyTool/"+toolname)
    tool.setDescription("a DummyTool instance owned by alg")
    tool.setLogLevel(4)

    global first_time
    if first_time:
        task.show()
        first_time = False
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

    global ith
    ith = 0
    global first_time
    first_time = True

    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(20000)

    # we will execute the HelloJob maximumly in 4 threads
    muster.config(HelloJob, 4)

    # TODO: show the configurations
    #muster.show()

    # spawn the threads and begin to run 
    muster.run()

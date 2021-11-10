def CoAnalyJob():
    import Sniper
    global ith
    ith += 1
    task = Sniper.Task(str(ith) +"-Job")

    import CoAnalysis
    x = task.createAlg("DummyAnalyAlg")
    


def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    task.createAlg("InputFragAlg")
    return task
def Goutput():
    import Sniper
    task = Sniper.Task("GOutput")

    return task

if __name__ == "__main__":

    import Sniper
    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    import SniperMuster
    muster = SniperMuster.Muster()

    gs = SniperMuster.createGlobalStream("FragmenStream/GFragStream")
    gs.configInput(GInput)
    gs.configOutput(GOutput)
    gs.configBuffer(50,20)

    
#!/usr/bin/env python

def BuildGraph():
    import SniperMuster
    g = SniperMuster.createWorker("MtDagTask")
    g.setDescription("an example to use algorithm dependence graph")

    # An algorithm dependence graph:
    #     C ------
    #    /         \
    #   B           \
    #  / \           H -- I
    # A   D ----    /
    #  \         \ /
    #   E -- F -- G
    # One correct sequence: A-B-E-C-D-F-G-H-I

    import SniperCoreUsages

    algI = g.insertNode("GraphTestAlg/algI")
    algH = g.insertNode("GraphTestAlg/algH")
    algG = g.insertNode("GraphTestAlg/algG")
    algF = g.insertNode("GraphTestAlg/algF")
    algE = g.insertNode("GraphTestAlg/algE")
    algD = g.insertNode("GraphTestAlg/algD")
    algC = g.insertNode("GraphTestAlg/algC")
    algB = g.insertNode("GraphTestAlg/algB")
    algA = g.insertNode("GraphTestAlg/algA")

    g.makeEdge("algA", "algB")
    g.makeEdge("algA", "algE")
    g.makeEdge("algB", "algC")
    g.makeEdge("algB", "algD")
    g.makeEdge("algC", "algH")
    g.makeEdge("algD", "algG")
    g.makeEdge("algE", "algF")
    g.makeEdge("algF", "algG")
    g.makeEdge("algG", "algH")
    g.makeEdge("algH", "algI")

    algA.property("INFO").set("I'm algA")
    algB.property("INFO").set("I'm algB")
    algC.property("INFO").set("I'm algC")
    algD.property("INFO").set("I'm algD")
    algE.property("INFO").set("I'm algE")
    algF.property("INFO").set("I'm algF")
    algG.property("INFO").set("I'm algG")
    algH.property("INFO").set("I'm algH")
    algI.property("INFO").set("I'm algI")

    g.done()
    return g


if __name__ == "__main__":

    import Sniper
    Sniper.setLogLevel(2)

    import SniperMuster
    muster = SniperMuster.Muster()

    muster.setEvtMax(5000)
    # set max number of threads
    muster.setThreads(8)

    muster.config(BuildGraph, 4)

    SniperMuster.show()
    muster.run()
def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    task.createAlg("InputFragAlg")
    return task


if __name__ == "__main__":

    import Sniper
    Sniper.setLogLevel(3)
    #Sniper.setShowTime()
    #Sniper.setLogFile("log.txt", False)

    import SniperMuster
    muster = SniperMuster.Muster()

    
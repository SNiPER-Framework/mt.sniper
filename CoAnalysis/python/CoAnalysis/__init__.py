import sys
sys.setdlopenflags( 0x100 | 0x2 ) # RTLD_GLOBAL | RTLD_NOW

import Sniper
Sniper.loadDll("libCoAnalysis.so")
del Sniper

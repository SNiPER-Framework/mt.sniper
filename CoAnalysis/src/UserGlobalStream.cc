#include "src/GlobalStream.h"
#include "SniperMuster/GlobalStreamFactory.h"
#include "SniperMuster/Fragment.h"
#include "EvtNavigator/EvtNavigator.h"

//BookGlobalStream MusterBook_GlobalStreamJMEvtNavigator_("GlobalStream<JM::EvtNavigator>", &MusterCreateGS<GlobalStream<JM::EvtNavigator>>);


typedef GlobalStream<Fragment<JM::EvtNavigator>> FragmentStream;

DECLARE_GLOBAL_STREAM(FragmentStream);//注册存放Fragment的GlobalStream



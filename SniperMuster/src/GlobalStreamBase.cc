#include "SniperMuster/GlobalStreamBase.h"

std::map<std::string, GlobalStreamBase *> GlobalStreamBase::s_GBufMap;

GlobalStreamBase::GlobalStreamBase(const std::string &name)
    : DLElement(name)
{
    s_GBufMap.insert(std::make_pair(name, this));
}

GlobalStreamBase::~GlobalStreamBase()
{
}

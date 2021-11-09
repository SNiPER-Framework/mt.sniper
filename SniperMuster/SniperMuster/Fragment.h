#ifndef FRAGMENT_H
#define FRAFMENT_H

#include "GlobalBuffer.h"
#include <deque>

template<typename T>
class Fragment{
public:
    //GlobalBuffer<T>::Elm fBegin;
    std::deque<std::shared_ptr<T>>  evtDeque;

    int lbegin;
    int lend;
};
#endif
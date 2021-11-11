#include "ThrdInputSvc.h"

bool ThrdInputSvc::initialize(){
    m_inputStream = new ReadFragBufStream();

    bool stat = m_inputStream->initialize();

    if( !stat ){
        LogError<<"Fialed to initialize the input stream"<<std::endl;
    }

    return stat;
}

bool ThrdInputSvc::finalize(){
    bool stat = m_inputStream->finalize();

    delete m_inputStream;

    return stat;
}

IInputStream* ThrdInputSvc::getInputStream(const std::string& /*path*/){
    return m_inputStream;
}
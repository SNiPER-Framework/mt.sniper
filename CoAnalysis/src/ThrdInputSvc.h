#ifndef THRD_INPUT_SVC
#define THRD_INPUT_SVC

#include "RootIOSvc/IInputSvc.h"
#include "SniperKernel/SvcBase.h"
#include "ReadFragBufStream.h"

class ThrdInputSvc: public SvcBase, public IInputSvc{
public:
    ThrdInputSvc(const std::string& name) : SvcBase(name) {}
    virtual ~ThrdInputSvc() {}
    
    bool initialize();
    bool finalize();

    IInputStream* getInputStream(const std::string& path);

private:
    std::string m_type;
    IInputStream* m_inputStream;
};

#endif

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperJSON.h"
#include "SniperMuster/GlobalStream4Any.h"
#include <memory>

typedef SniperJSON JsonEvent;
typedef std::map<std::string, std::any> MappedEvent;

class IFillGlobalBufSvc
{
public:
    virtual void fill(std::shared_ptr<JsonEvent> &pevt) = 0;
    virtual void stop() = 0;
};

class IGetGlobalBufSvc
{
public:
    virtual MappedEvent &get() = 0;
    virtual MappedEvent &pop() = 0;
    virtual void done() = 0;
};

//////////////////////////////////////////////////////////////////////////////////
class OldStyleFillGlobalBufSvc : public SvcBase, public IFillGlobalBufSvc
{
    public:
        OldStyleFillGlobalBufSvc(const std::string &name);
        virtual ~OldStyleFillGlobalBufSvc() = default;

        virtual bool initialize() override;
        virtual bool finalize() override { return true; }

        virtual void fill(std::shared_ptr<JsonEvent> &pevt);
        virtual void stop();
    private:
        GlobalBuffer4Any* m_gbuf;
};
SNIPER_DECLARE_DLE(OldStyleFillGlobalBufSvc);

OldStyleFillGlobalBufSvc::OldStyleFillGlobalBufSvc(const std::string &name)
    : SvcBase(name)
{
}

bool OldStyleFillGlobalBufSvc::initialize()
{
    m_gbuf = GlobalStream4Any::GetBufferFrom("GlobalStream");
    return true;
}

void OldStyleFillGlobalBufSvc::fill(std::shared_ptr<JsonEvent> &pevt)
{
    auto v = new GlobalBuffer4Any::value_type{{"event", pevt}};
    m_gbuf->push_back(std::shared_ptr<GlobalBuffer4Any::value_type>(v));
}

void OldStyleFillGlobalBufSvc::stop()
{
    m_gbuf->push_back(nullptr);
    getParent()->stop();
}

//////////////////////////////////////////////////////////////////////////////////
class OldStyleGetGlobalBufSvc : public SvcBase, public IGetGlobalBufSvc
{
    public:
        OldStyleGetGlobalBufSvc(const std::string &name);
        virtual ~OldStyleGetGlobalBufSvc() = default;

        virtual bool initialize() override;
        virtual bool finalize() override { return true; }

        virtual MappedEvent &get() override;
        virtual MappedEvent &pop() override;
        virtual void done() override;
    private:
        GlobalBuffer4Any* m_gbuf;
        GlobalBuffer4Any::Elem* m_slot;
};
SNIPER_DECLARE_DLE(OldStyleGetGlobalBufSvc);

OldStyleGetGlobalBufSvc::OldStyleGetGlobalBufSvc(const std::string &name)
    : SvcBase(name)
{
}

bool OldStyleGetGlobalBufSvc::initialize()
{
    m_gbuf = GlobalStream4Any::GetBufferFrom("GlobalStream");
    return true;
}

MappedEvent &OldStyleGetGlobalBufSvc::get()
{
    m_slot = m_gbuf->next();
    if (m_slot == nullptr)
    {
        m_par->stop();
    }
    return *(m_slot->dptr);
}

MappedEvent &OldStyleGetGlobalBufSvc::pop()
{
    static std::shared_ptr<GlobalBuffer4Any::value_type> data;
    data = m_gbuf->pop_front();
    if (data == nullptr)
    {
        m_par->stop();
    }
    return *data;
}

void OldStyleGetGlobalBufSvc::done()
{
    m_gbuf->setDone(m_slot);
}

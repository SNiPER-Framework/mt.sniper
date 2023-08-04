#include "SniperKernel/SvcBase.h"
#include "SniperKernel/IIncidentHandler.h"
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
        virtual bool finalize() override;

        virtual MappedEvent &get() override;
        virtual MappedEvent &pop() override;
    private:
        class EndEvtHandler : public IIncidentHandler
        {
            public:
                EndEvtHandler(ExecUnit *domain, OldStyleGetGlobalBufSvc* svc)
                    : IIncidentHandler(domain),
                      m_svc(svc)
                {
                }
                virtual ~EndEvtHandler() = default;

                virtual bool handle(Incident &incident) override;

            private:
                OldStyleGetGlobalBufSvc *m_svc;
        };

        friend class EndEvtHandler;

        GlobalBuffer4Any* m_gbuf;
        GlobalBuffer4Any::Elem* m_slot{nullptr};
        EndEvtHandler* m_handler;
};
SNIPER_DECLARE_DLE(OldStyleGetGlobalBufSvc);

OldStyleGetGlobalBufSvc::OldStyleGetGlobalBufSvc(const std::string &name)
    : SvcBase(name)
{
}

bool OldStyleGetGlobalBufSvc::initialize()
{
    m_gbuf = GlobalStream4Any::GetBufferFrom("GlobalStream");
    m_handler = new EndEvtHandler(m_par, this);
    m_handler->regist("EndEvent");
    return true;
}

bool OldStyleGetGlobalBufSvc::finalize()
{
    m_handler->unregist("EndEvent");
    delete m_handler;
    return true;
}

MappedEvent &OldStyleGetGlobalBufSvc::get()
{
    if (m_slot == nullptr)
    {
        m_slot = m_gbuf->next();
        if (m_slot == nullptr)
        {
            m_par->stop();
        }
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

bool OldStyleGetGlobalBufSvc::EndEvtHandler::handle(Incident &/*incident*/)
{
    if (m_svc->m_slot != nullptr)
    {
        m_svc->m_gbuf->setDone(m_svc->m_slot);
        m_svc->m_slot = nullptr;
    }

    return true;
}

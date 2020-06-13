#ifndef MANDIS_MANDIS_P2P_REQUEST_HANDLER_H_
#define MANDIS_MANDIS_P2P_REQUEST_HANDLER_H_

#include <boost/noncopyable.hpp>

namespace p2pnet {
    struct reply;
    struct request;

    class RequestHandler : private boost::noncopyable
    {
    public:
        explicit RequestHandler();
        
        void HandleRequest(const request &req, reply& rep);

    private:
    };
}

#endif // !MANDIS_MANDIS_P2P_REQUEST_HANDLER_H_


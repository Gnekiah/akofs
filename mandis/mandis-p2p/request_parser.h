#ifndef MANDIS_MANDIS_P2P_REQUEST_PARSER_H_
#define MANDIS_MANDIS_P2P_REQUEST_PARSER_H_

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace p2pnet {
    struct Request;

    class RequestParser {
    public:
        RequestParser();

        void Reset();

        int Parse(Request& req, char* buff, size_t offset) {
            return 1;
        }

    private:/*
        boost::tribool Consume(Request &req, char input);
        static bool IsChar(int c);
        static bool IsCtrl(int c);
        static bool IsTspecial(int c);
        static bool IsDigit(int c);
         */
        enum state {

        } state_;
    };


}

#endif // !MANDIS_MANDIS_P2P_REQUEST_PARSER_H_


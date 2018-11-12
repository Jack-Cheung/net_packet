#include "header.hpp"
#include "global.hpp"
#include <memory.h>

ostream& operator<<(ostream& os, Header& header)
{
    CODE_PIECE_HEADER_OSTREAM
    return os;
}

istream& operator>>(istream& is, Header& header)
{
    CODE_PIECE_HEADER_ISTREAM
    return is;
}

Header::Header()
{
    CODE_PIECE_HEADER_CTOR
}

Header::Header(initializer_list<uint64_t> l)
{
    //tmp
    auto itr = l.begin();
    auto end = l.end();
    version[0] = *itr++;
    operation[0] = *itr++;
    unfixed_size[0] = *itr++;
    open_session[0] = *itr;
}

void Header::prettyPrint(ostream& os)
{
    os << "HEADER:\n";
    uint64_t buf_size = 0;
    CODE_PIECE_HEADER_PRINTER
}
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

void Header::prettyPrint(ostream& os)
{
    os << "HEADER:\n";
    uint64_t buf_size = 0;
    CODE_PIECE_HEADER_PRINTER
}
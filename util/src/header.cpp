#include "header.hpp"
#include "global.hpp"
#include <memory.h>

ostream& operator<<(ostream& os, Header& header)
{
    os.write((const char*)&header.version, sizeof(header.version))
    .write((const char*)&header.operation, sizeof(header.operation))
    .write((const char*)&header.unfixed_size, sizeof(header.unfixed_size))
    .write((const char*)&header.packet_len, sizeof(header.packet_len))
    .write((const char*)&header.open_session, sizeof(header.open_session));
    return os;
}

istream& operator>>(istream& is, Header& header)
{
    is.read((char*)&header.version, sizeof(header.version))
    .read((char*)&header.operation, sizeof(header.operation))
    .read((char*)&header.unfixed_size, sizeof(header.unfixed_size))
    .read((char*)&header.packet_len, sizeof(header.packet_len))
    .read((char*)&header.open_session, sizeof(header.open_session));
    return is;
}

Header::Header():packet_len(0)
{
    ::memset(version, 0, sizeof(version));
    ::memset(operation, 0, sizeof(operation));
    ::memset(unfixed_size, 0, sizeof(unfixed_size));
    ::memset(open_session, 0, sizeof(open_session));
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
    os << "version\t\t";
    uint64_t buf_size = 0;
    const char* buf = get_hex_string(version , sizeof(version), buf_size);
    os << buf;
    delete[] buf;
    os << endl;
    os << "operation code\t\t";
    buf =  get_hex_string(operation , sizeof(operation), buf_size);
    os << buf;
    delete[] buf;
    os << endl;
    os << "unfixed size\t\t";
    buf =  get_hex_string(unfixed_size , sizeof(unfixed_size), buf_size);
    os << buf;
    delete[] buf;
    os << endl;
    os << "packet length\t\t";
    os << to_string(packet_len) << endl;
    os << "if open session\t\t";
    for(uint64_t i = 0; i < sizeof(open_session); ++i)
    {
        os << hex << open_session[i];
    }
    os << endl;
}
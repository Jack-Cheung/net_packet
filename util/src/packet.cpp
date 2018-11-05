#include "packet.hpp"
#include <string>
#include <fstream>
#include <cstdio>
#include <memory.h>
#include <initializer_list>

const char* get_hex_string(const uint8_t* data, uint32_t len, uint64_t& buf_siz)
{
    uint8_t step = 3;
    uint64_t buffer_size = step * len + 1;
    buf_siz = buffer_size;
    char* buffer = new char[buffer_size];
    ::memset(buffer, 0, buffer_size);
    uint32_t cnt = 0;
    uint32_t buf_cursor = 0;
    while(cnt < len)
    {
        std::sprintf(buffer + buf_cursor, "%02X|", *(data + cnt));
        ++cnt;
        buf_cursor += step;
    }
    return buffer;
}

ostream& operator<<(ostream& os, Header& header)
{
    os.write((const char*)&header.version, sizeof(header.version))
    .write((const char*)&header.operation, sizeof(header.operation))
    .write((const char*)&header.unfixed_size, sizeof(header.unfixed_size))
    .write((const char*)&header.packet_len, sizeof(header.packet_len))
    .write((const char*)&header.open_session, sizeof(header.open_session));
    return os;
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

ostream& operator<<(ostream& os, const Param& param)
{
    os.write((const char*)&param.len, sizeof(param.len));
    for(uint32_t i = 0; i < param.len; ++i)
    {
        os.write((const char*)(param.data + i), sizeof(uint8_t));
    }
    return os;
}

ostream& operator<<(ostream& os, Packet& packet)
{
    os << packet.header;
    for(const auto &p : packet.params)
    {
        os << *p;
    }
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

istream& operator>>(istream& is, Packet& packet)
{
    is >> packet.header;
    uint64_t packet_len = packet.header.packet_len;
    uint64_t cnt = Header::HEADER_LEN;
    while(cnt < packet_len)
    {
        uint32_t param_len = 0;// to do
        is.read((char*)&param_len, sizeof(param_len));
        Param* p = nullptr;
        if(1 == param_len)
        {
            uint8_t data;
            is.read((char*)&data, sizeof(data));
            packet.params.push_back(unique_ptr<Param>(new Param(&data, param_len)));
        }
        else if(param_len < 1)
        {
            throw std::logic_error(" param len = " + to_string(param_len));
        }
        else
        {
            uint8_t* arr = new uint8_t[param_len];
            is.read((char*)arr, param_len);
            packet.params.push_back(unique_ptr<Param>(new Param(arr, param_len)));
            delete[] arr;
        }
        cnt += param_len + Param::PARAM_LEN_LEN;
    }
}



void Header::prettyPrint(ostream& os)
{
    os << "HEADER:\n";
    os << "version\t\t";
    /* for(uint64_t i = 0; i < sizeof(version); ++i)
    {
        os << hex << version[i];
    } */
    uint64_t buf_size = 0;
    const char* buf = get_hex_string(version , sizeof(version), buf_size);
    os << buf;
    delete[] buf;
    os << endl;
    os << "operation code\t\t";
    /* for(uint64_t i = 0; i < sizeof(operation); ++i)
    {
        os << hex << operation[i];
    } */
    buf =  get_hex_string(operation , sizeof(operation), buf_size);
    os << buf;
    delete[] buf;
    os << endl;
    os << "unfixed size\t\t";
    /* for(uint64_t i = 0; i < sizeof(unfixed_size); ++i)
    {
        os << hex << unfixed_size[i];
    } */
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

Param::Param(uint8_t data)
{
    this->data = new uint8_t(data);
    len = 1;
}

Param::Param(uint8_t* data, uint32_t size)
{
    this->len = size;
    this->data = new uint8_t[len];
    std::copy(data, data + size, this->data);
}

Param::Param(const string& str)
{
    this->len = str.size();
    this->data = new uint8_t[len];
    std::copy(str.begin(),str.end(), data);
}

Param::~Param()
{   
    if(data == nullptr)
    {
        return;
    }
    if(len == 1)
    {
        delete data;
    }
    else
    {
        delete[] data;
    }
}

//to do
Param& Param::operator=(Param&& p)
{
    if(this != &p)
    {
        this->data = p.data;
        p.data = nullptr;
        this->len = p.len;
        p.len = 0;
    }
    return *this;
}

void Param::prettyPrint(ostream& os)
{
    os << "PARAM [len = " << to_string(len) << "]\n";
    os << "\thex content = { ";
    uint64_t buffer_size = 0;
    const char* buffer = get_hex_string(data, len, buffer_size);
    os.write((const char*)buffer, buffer_size);
    os << " }\n";
    delete[] buffer;
}

Packet::Packet(const vector<char>& v)
{
    stringstream ss;
    for(auto& c : v)
    {
        ss.write(&c, 1);
    }
    ss >> *this;
}

Packet::Packet(char* data, uint64_t size)
{
    stringstream ss;
    ss.write(data, size);
    ss >> *this;
}

void Packet::setHeader(const Header& header)
{
    this->header = header;
    this->header.packet_len = Header::HEADER_LEN;
}

const Header& Packet::getHeader()
{
    return this->header;
}

const Param& Packet::getParam(uint8_t idx)
{
    return *(this->params.at(idx));
}

Packet&  Packet::addParam(Param& param)
{
    this->params.push_back(unique_ptr<Param>(&param));
    this->header.packet_len += param.len + Param::PARAM_LEN_LEN;
    return *this;
}

void Packet::serialize(vector<char>& vec)
{
    stringstream ss;
    ss << *this;
    char c;
    while(ss)
    {
        c = 0;
        ss.read(&c, 1);
        vec.push_back(c);
    }
    vec.pop_back();//to do
}

uint64_t Packet::serialize(char* arr)
{
    stringstream ss;
    ss << *this;
    char c;
    uint64_t cnt = 0;
    while(ss)
    {
        c = 0;
        ss.read(&c, 1);
        *(arr + cnt) = c;
        cnt++;
    }
    return cnt - 1;
}

void Packet::prettyPrint(ostream& os)
{
    os << string(64, '=') << endl;
    header.prettyPrint(os);
    os << string(64, '-') << endl;
    uint8_t cnt = 0;
    for(auto& p : params)
    {
        os << "#" + to_string(++cnt) + "  " ;
        p->prettyPrint(os);
    }
}
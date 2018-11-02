#include "packet.hpp"
#include <string>
#include <fstream>
#include <cstdio>
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

ostream& operator<<(ostream& os, const Param& param)
{
    os.write((const char*)&param.len, sizeof(param.len));
    for(int i = 0; i < param.len; ++i)
    {
        os.write((const char*)(param.data + i),sizeof(uint8_t));
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
    uint64_t cnt = HEADER_LEN;
    while(cnt < packet_len)
    {
        uint8_t param_len = 0;
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
            //arr[param_len] = '\0';
            packet.params.push_back(unique_ptr<Param>(new Param(arr, param_len)));
            delete[] arr;
        }
        cnt += param_len + PARAM_LEN_LEN;
    }
}

void Header::prettyPrint(ostream& os)
{
    os << "HEADER:\n";
    os << "version\t\t" << to_string(version)  << endl;
    os << "operation code\t\t" << to_string(operation) << endl;
    os << "unfixed size\t\t" << to_string(unfixed_size) << endl;
    os << "packet length\t\t" << to_string(packet_len) << endl;
    os << "if open session\t\t" << to_string(open_session) << endl;
}

Param::Param(uint8_t data)
{
    this->data = new uint8_t(data);
    len = 1;
}

Param::Param(uint8_t* data, uint64_t size)
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
    if(len == 1)
    {
        delete data;
    }
    else
    {
        delete[] data;
    }
}

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
    uint8_t step = 3;
    int buffer_size = step * len + 1;
    char* buffer = new char[buffer_size];
    ::memset(buffer, 0, buffer_size);
    uint8_t cnt = 0;
    uint8_t buf_cursor = 0;
    
    while(cnt < len)
    {
        std::sprintf(buffer + buf_cursor, "%02X ", *(data + cnt));
        ++cnt;
        buf_cursor += step;
    }
    os.write((const char*)buffer, buffer_size);
    os << " }\n";
    /* os << "\tcontent = { ";
    os.write((const char*)data, len);
    os << " }\n"; */
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
    this->header.packet_len = HEADER_LEN;
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
    this->header.packet_len += param.len + PARAM_LEN_LEN;
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
    //to do
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
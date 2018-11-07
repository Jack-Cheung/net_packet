#include "packet.hpp"
#include <string>
#include <fstream>
#include <cstdio>
#include <memory.h>
#include <initializer_list>

ostream& operator<<(ostream& os, Packet& packet)
{
    os << packet.header;
    for(const auto &p : packet.params)
    {
        os << *p;
    }
    return os;
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
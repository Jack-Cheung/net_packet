#include "packet.hpp"
#include <string>
ostream& operator<<(ostream& os, Header& header)
{
    //os << header.version  << header.operation << header.unfixed_size;
    os.write((const char*)&header.version, sizeof(header.version))
    .write((const char*)&header.operation, sizeof(header.operation))
    .write((const char*)&header.unfixed_size, sizeof(header.unfixed_size))
    .write((const char*)&header.packet_len, sizeof(header.packet_len))
    .write((const char*)&header.open_session, sizeof(header.open_session));
    //os << "---------" << header.packet_len << "---------" << header.open_session;
    //cout << header.packet_len << "000" << to_string(header.packet_len) << endl;
    return os;
}

ostream& operator<<(ostream& os, const Param& param)
{
    //os << param.len;
    os.write((const char*)&param.len, sizeof(param.len));
    for(int i = 0; i < param.len; ++i)
    {
        //os << *(param.data + i);
        os.write((const char*)(param.data + i),sizeof(uint8_t));
    }
    return os;
}

ostream& operator<<(ostream& os, Packet& packet)
{
    packet.header.packet_len = packet.len;
    os << packet.header;
    for(const auto &p : packet.params)
    {
        os << *p;
    }
    return os;
}

istream& operator>>(istream& is, Header& header)
{
    
    is >> header.version >> header.operation >> header.unfixed_size
      >> header.packet_len >> header.open_session;
    return is;
}

/* istream& operator>>(istream& is, Param& param)
{
    is >> param.len;
    uint8_t* ptr = param.data;
    for(int i = 0; i < param.len; ++i)
    {
        is >> *ptr;
        ++ptr;
    }
    return is;
} */

istream& operator>>(istream& is, Packet& packet)
{
    is >> packet.header;
    uint64_t packet_len = packet.header.packet_len;
    uint64_t cnt = HEADER_LEN;
    while(cnt < packet_len)
    {
        uint8_t param_len = 0;
        is >> param_len;
        Param* p = nullptr;
        if(1 == param_len)
        {
            uint8_t data;
            is >> data;
        }
        else
        {
            uint8_t* arr = new uint8_t[param_len];
            for(uint64_t i = 0; i < param_len; i++)
            {
                uint8_t data;
                is >> data;
                *(arr + i) = data;
            }
            string str((char*)arr);
            Param* p = new Param(str);
            packet.params.push_back(p);
            delete[] arr;
        }
        cnt += param_len;
    }
}

Param::Param(uint8_t data)
{
    this->data = new uint8_t(data);
    len = 1;
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

void Packet::setHeader(const Header& header)
{
    this->header = header;
}

Packet::Packet():len(HEADER_LEN)
{

}

Packet::~Packet()
{
    for(auto& p : params)
    {
        if(p)
        {
            delete p;
            p = nullptr;
        }
    }
}

Packet&  Packet::addParam(Param* param)
{
    this->params.push_back(param);
    this->len += param->len;
    return *this;
}
//todo
/* void Packet::addParam(std::initializer_list<pair<string,Param>> list)
{
    for(auto it: list)
    {
        this->params.insert(it);
    }
    
    for(auto itr = list.begin(); itr < list.end(); ++itr)
    {
        this->params.insert(*itr);
    }
    
} */
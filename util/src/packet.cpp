#include "packet.hpp"
#include <string>

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
        is.read((char*)&param_len, sizeof(param_len));
        Param* p = nullptr;
        if(1 == param_len)
        {
            uint8_t data;
            is.read((char*)&data, sizeof(data));
        }
        else
        {
            uint8_t* arr = new uint8_t[param_len];
            for(uint64_t i = 0; i < param_len; i++)
            {
                uint8_t data;
                is.read((char*)&data, sizeof(data));
                *(arr + i) = data;
            }
            arr[param_len] = '\0';
            packet.params.push_back(unique_ptr<Param>(new Param(arr, param_len)));
            delete[] arr;
        }
        cnt += param_len + PARAM_LEN_LEN;
    }
}

Param::Param(uint8_t data)
{
    this->data = new uint8_t(data);
    len = 1;
}

Param::Param(uint64_t param)
{
    len = sizeof(param);
    this->data = new uint8_t[len];
    uint8_t* p = (uint8_t*)&param;
    std::copy(p, p + len, data);
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

Packet::Packet(const vector<char>& v)
{
    stringstream ss;
    for(auto& c : v)
    {
        ss << c;
    }
    ss >> *this;
}

Packet::Packet(char* data, uint64_t size)
{
    stringstream ss;
    for(uint64_t i = 0; i < size; ++i)
    {
        ss << data[i];
    }
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
    return cnt;
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
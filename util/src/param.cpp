#include "param.hpp"

ostream& operator<<(ostream& os, const Param& param)
{
    os.write((const char*)&param.len, sizeof(param.len));
    for(uint32_t i = 0; i < param.len; ++i)
    {
        os.write((const char*)(param.data + i), sizeof(uint8_t));
    }
    return os;
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
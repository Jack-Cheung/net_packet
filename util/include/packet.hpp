#pragma once
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <memory.h>

using namespace std;

//need free the dynamic memory after usage
const char* get_hex_string(const uint8_t* data, uint32_t len, uint64_t& buf_siz);


struct Header
{
    friend class Packet;
    Header();
    Header(initializer_list<uint64_t> l);// test
    const static int HEADER_LEN = 12;
    uint8_t version[1];
    uint8_t operation[1];
    uint8_t unfixed_size[1];
    uint64_t packet_len; //special
    uint8_t open_session[1];
    void prettyPrint(ostream& os);
    friend ostream& operator<<(ostream& os, Header& header);
    friend istream& operator>>(istream& is, Header& header);
};

struct Param
{
    friend class Packet;
    Param(uint8_t);
    template <typename T>
    //internal data type
    Param(T param)
    {
        len = sizeof(param);
        this->data = new uint8_t[len];
        uint8_t* p = (uint8_t*)&param;
        std::copy(p, p + len, data);
    }
    Param(const string&);
    Param(uint8_t*, uint32_t size);
    ~Param();
    void prettyPrint(ostream& os);
    Param& operator=(Param&& p);
    friend ostream& operator<<(ostream& os, const Param& param);
    const static int PARAM_LEN_LEN = 4;
private:
    uint32_t len;
protected:
    uint8_t* data;
};


struct Packet
{
    Packet() = default;
    ~Packet() = default;
    Packet(const vector<char>&);
    Packet(char*, uint64_t);
    void setHeader(const Header& header);
    const Header& getHeader();
    const Param& getParam(uint8_t idx);
    Packet& addParam(Param& param);
    void serialize(vector<uint8_t>&);
    void serialize(vector<char>&);
    uint64_t serialize(char*);
    void prettyPrint(ostream& os);
private:
    Header header;
    vector<unique_ptr<Param>> params;
    uint64_t len;
    friend ostream& operator<<(ostream& os, Packet& packet);
    friend istream& operator>>(istream& is, Packet& packet);
};



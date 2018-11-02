#pragma once
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct Header
{
    friend class Packet;
    const static int HEADER_LEN = 12;
    uint8_t version;
    uint8_t operation;
    uint8_t unfixed_size;
    uint64_t packet_len;
    uint8_t open_session;
    void prettyPrint(ostream& os);
    friend ostream& operator<<(ostream& os, Header& header);
    friend istream& operator>>(istream& is, Header& header);
};

struct Param
{
    friend class Packet;
    Param(uint8_t);
    template <typename T>
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



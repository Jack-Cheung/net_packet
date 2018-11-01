#pragma once
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#define HEADER_LEN  12
#define PARAM_LEN_LEN 1

using namespace std;



struct Header
{
    friend class Packet;
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
    Param(uint64_t);
    Param(const string&);
    Param(uint8_t*, uint64_t size);
    ~Param();
    void prettyPrint(ostream& os);
    Param& operator=(Param&& p);
    friend ostream& operator<<(ostream& os, const Param& param);
private:
    uint8_t len;
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

//to do
    //Packet& addParam(Param& param);

    //void addParam(std::initializer_list<pair<string,Param>> list);

    //boost::shared_ptr<uint8_t[]> data; todo



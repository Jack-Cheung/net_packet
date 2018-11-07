#pragma once
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <memory.h>
#include "header.hpp"
#include "param.hpp"

using namespace std;

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
    Packet& addParam(Param&& param);
    //void addParam(initializer_list<> list);
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



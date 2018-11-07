#pragma once
#include <stdint.h>
#include <initializer_list>
#include <iostream>

using namespace std; 

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
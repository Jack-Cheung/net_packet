#pragma once
#include <stdint.h>
#define HEADER_PROPERTY(type, name)  type name;  \

#define HEADER_PROPERTY_LIST
HEADER_PROPERTY(uint8_t, version)
HEADER_PROPERTY(uint8_t, unfixed_size) 
HEADER_PROPERTY(uint64_t, packet_len)  //special
HEADER_PROPERTY(uint8_t, open_session)

#define _PARAM_LEN_LEN  8 //byte



struct Param
{
    friend class Packet;
    const static int PARAM_LEN_LEN = 4;
private:
    uint32_t len;
protected:
    uint8_t* data;
};


struct Header
{
    friend class Packet;
    const static int HEADER_LEN = 12;

    HEADER_PROPERTY_LIST
};
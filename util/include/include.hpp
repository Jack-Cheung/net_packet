#pragma once
#include <stdint.h>
#include <string>




#define LO_BYTE(var, bits)  

#define HEADER_PROPERTY(type, varname, bytes)  type name;  \
        const static uint64_t varname##_NEEDBITS = bytes;
#define HEADER_PROPERTY_LIST
HEADER_PROPERTY(uint8_t, version, 1)
HEADER_PROPERTY(uint8_t, unfixed_size, 1) 
HEADER_PROPERTY(uint64_t, packet_len, 8)  //special meanings  
HEADER_PROPERTY(uint8_t, open_session, 1)

#define VALUE_PARAM_LEN_LEN  8 //byte
#define VALUE_HEADER_LEN 12
//#define STR  #VALUE_PARAM_LEN_LEN

struct Param
{
    friend class Packet;
    const static int PARAM_LEN_LEN = VALUE_PARAM_LEN_LEN;
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
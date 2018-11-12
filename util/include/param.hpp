#pragma once
#include "global.hpp"
#include "generated.hpp"
#include <iostream>


using namespace std;

struct Param
{
    friend class Packet;

    Param(uint8_t);
    template <typename INTERNAL_TYPE>
    Param(INTERNAL_TYPE param)
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
    Param(Param&& p);
    Param& operator=(Param&& p);
    friend ostream& operator<<(ostream& os, const Param& param);
    const static int PARAM_LEN_LEN = VALUE_PARAM_LEN_LEN;
private:
    uint32_t len;
    uint8_t* data;
};
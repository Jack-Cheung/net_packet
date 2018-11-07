#pragma once
#include "global.hpp"
#include <iostream>

using namespace std;

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
    Param(Param&& p);
    Param& operator=(Param&& p);
    friend ostream& operator<<(ostream& os, const Param& param);
    const static int PARAM_LEN_LEN = 4;
private:
    uint32_t len;
    uint8_t* data;
};
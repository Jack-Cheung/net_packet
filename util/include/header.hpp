#pragma once
#include <stdint.h>
#include <initializer_list>
#include <iostream>
#include "generated.hpp"

using namespace std; 

struct Header
{
    friend class Packet;
    Header();
    const static int HEADER_LEN = VALUE_HEADER_LEN;
    HEADER_PROPERTY_LIST
    void prettyPrint(ostream& os);
    friend ostream& operator<<(ostream& os, Header& header);
    friend istream& operator>>(istream& is, Header& header);
};
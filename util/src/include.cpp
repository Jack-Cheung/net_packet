
/* #include <stdint.h>
#include <string>
#include <fstream>
#include <cstdio>
#include <memory.h>
#include <initializer_list>
#include <memory>
#include <vector>

using namespace std;

#define HEADER_PKGLEN_VAR_NAME packet_len 

#define HEADER_PROPERTY(varname, bytes) uint8_t varname [ bytes ];  \

#define HEADER_PKGLEN_PROPERTY(varname) uint64_t varname;
#define HEADER_PROPERTY_LIST     \
HEADER_PROPERTY(version, 1)      \
HEADER_PROPERTY(unfixed_size, 1) \ 
HEADER_PKGLEN_PROPERTY(HEADER_PKGLEN_VAR_NAME) \
HEADER_PROPERTY(open_session, 1)

#define VALUE_PARAM_LEN_LEN  8 //byte
#define VALUE_HEADER_LEN 12

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
    const static int HEADER_LEN = VALUE_HEADER_LEN;
    HEADER_PROPERTY_LIST
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
}; */
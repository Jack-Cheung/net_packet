#include "../util/include/packet.hpp"
#include <vector>
using namespace std;
int main(int argc, char const *argv[])
{
   Packet pkt;
   //fill the header
   Header header;
   header.open_session[0] = 1;
   header.operation[0] = 2;
   header.unfixed_size[0] = 3;
   header.version[0] = 4;
   pkt.setHeader(header);
   // add params
    uint8_t param1[1] = {1};
    uint8_t param2[8] = {1, 2, 3, 4, 0};
    pkt.addParam(Param(param1,sizeof(param1)))
        .addParam(Param(param2, sizeof(param2)));
    //get the binary data of the package
    vector<char> buf1;// make use of this , usually transimit to network
    pkt.serialize(buf1);
    char buf2[512] = {0};//array of characters
    pkt.serialize(buf2);
    pkt.prettyPrint(cout);//print the content of the package
   return 0;
}
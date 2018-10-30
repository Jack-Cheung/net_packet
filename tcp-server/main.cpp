#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include "../util/include/packet.hpp"


using boost::asio::ip::tcp;

const int max_length = 1024;

const int  REGISTER_CODE = 0b10000001; 

uint64_t reply_register(char* data)
{
    memset(data, 0, max_length);
    Packet pkt;
    Header header = {1,0b10000001,0,0,0};
    pkt.setHeader(header);
    string primaryPublicKey = "surpernodepublickey";
    string servicePublicKey = "servicepublickey";
    pkt.addParam(*new Param(primaryPublicKey))
        .addParam(*new Param(servicePublicKey));
    uint64_t length;
    cout << "haha len=" << (length = pkt.serialize(data));
    return length;
}
void reply_report()
{

}
void session(tcp::socket sock)
{
  try
  {
    char data[max_length];
    boost::system::error_code error;
    uint64_t length = sock.read_some(boost::asio::buffer(data), error);
    std::cout << length << std::endl;
    /* if (error == boost::asio::error::eof)
        return;
    else if (error)
        throw boost::system::system_error(error); */ // Some other error.
    Packet pkt(data, length);
    switch(pkt.getHeader().operation)
    {
        case REGISTER_CODE:
            reply_register(data);
            break;
        default:
            break;
    }

    length = reply_register(data);
    for(size_t i = 0; i < length; i++)
    {
        std::cout << data[i];
    }
    std::cout << std::endl;
    boost::asio::write(sock, boost::asio::buffer(data, length));
    //std::cout << data << endl;
    /* ofstream os;
    os.open("test1.txt", fstream::out | fstream::binary);
    os << pkt;
    os.close(); */
    std::cout << "thread finished" << std::endl;
   }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    std::cout << "create a thread";
    std::cout.flush();
    std::thread(session, a.accept()).detach();
  }
}

int main(int argc, char* argv[])
{
  try
  {

    boost::asio::io_context io_context;

    server(io_context, 6688);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

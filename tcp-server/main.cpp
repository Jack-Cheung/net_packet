#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include "../util/include/packet.hpp"


using boost::asio::ip::tcp;

const int  max_length = 1024;
const int  REGISTER_CODE = 0b10000001; 
const int  REPORT_CODE = 0b00000010;
const int  EXAM_CODE = 0b00000011;
const int  SENDFILE_CODE = 0b00000100;
const int  SAVEFILE_CODE = 0b00000101;
const int  DOWNLOAD_CODE = 0b00000110;
const int  GETBLOCKBEG = 0b00001000;
const int  GETBLOCKEND = 0b00001001;

uint64_t reply_register(char* data)
{
    memset(data, 0, max_length);
    Packet pkt;
    Header header = {1,REGISTER_CODE,0,0,0};
    pkt.setHeader(header);
    string primaryPublicKey = "surpernodepublickey";
    string servicePublicKey = "servicepublickey";
    pkt.addParam(*new Param(primaryPublicKey))
        .addParam(*new Param(servicePublicKey));
    uint64_t length = pkt.serialize(data);
    //pkt.prettyPrint(cout);
    return length;
}
uint64_t reply_report(char* data)
{
    //todo
    memset(data, 0, max_length);
    Packet pkt;
    Header header = {1, REPORT_CODE,0,0,0};
    pkt.setHeader(header);
    string primaryPublicKey = "surpernodepublickey";
    string servicePublicKey = "servicepublickey";
    pkt.addParam(*new Param(primaryPublicKey))
        .addParam(*new Param(servicePublicKey));
    uint64_t length = pkt.serialize(data);
    pkt.prettyPrint(cout);
    return length;
}
uint64_t reply_exam(char* data)
{
    memset(data, 0, max_length);
    Packet pkt;
    Header header = {1, EXAM_CODE,0,0,0};
    pkt.setHeader(header);
    int64_t examser = 1;
    int32_t examlen = 2;
    string signature = "this is a signature";
    int16_t fileid = 888;
    int8_t postAction = 1;
    int8_t count = 1;
    pkt.addParam(*new Param(examser))
        .addParam(*new Param(examlen))
        .addParam(*new Param(signature))
        .addParam(*new Param(fileid))
        .addParam(*new Param(postAction))
        .addParam(*new Param(count))
        ;
    uint64_t length = pkt.serialize(data);
    pkt.prettyPrint(cout);
    return length;
}

uint64_t reply_sendfile(char* data)
{
    memset(data, 0, max_length);
    Packet pkt;
    Header header = {1, SENDFILE_CODE,0,0,0};
    pkt.setHeader(header);
    string signature = "this is a signature";
    pkt.addParam(*new Param(signature));
    uint64_t length = pkt.serialize(data);
    pkt.prettyPrint(cout);
    return length;
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
    pkt.prettyPrint(cout);
    switch(pkt.getHeader().operation)
    {
        case REGISTER_CODE:
            length = reply_register(data);
            break;
        default:
            break;
    }


    boost::asio::write(sock, boost::asio::buffer(data, length));
    Packet pkt1(data, length);
    pkt1.prettyPrint(cout);
    std::cout << "thread finished" << std::endl;
   }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void session_pessive(tcp::socket sock, int cmd)
{
  try
  {
    char data[max_length];
    uint64_t length = 0;
    switch (cmd)
    {
        case REPORT_CODE:
            {
                length = reply_report(data);
            }
            break;
        case EXAM_CODE:
            length = reply_exam(data);
            break;
        case SENDFILE_CODE:
            {
                length = reply_sendfile(data);
                break;
            }
        default:
            break;
    }
    length = sock.write_some(boost::asio::buffer(data, length));

    boost::system::error_code error;
    length = boost::asio::read(sock, boost::asio::buffer(data), error);
    std::cout << "Resive :" << std::endl;
    Packet pkt(data, length);
    pkt.prettyPrint(cout);
    std::cout << "thread finished" << std::endl;
   }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}


void server(boost::asio::io_context& io_context, unsigned short port, int cmd,  bool positive = true)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    std::cout << "create a thread" << std::endl;
    std::cout.flush();
    switch (cmd)
    {
        case REPORT_CODE:
        case EXAM_CODE:
        case SENDFILE_CODE:
            std::thread(session_pessive, a.accept(), cmd).detach();
            break;
    
        default:
            std::thread(session, a.accept()).detach();
            break;
    }
  }
}

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;
    if (argc == 2) 
    {
        std::cout << "pessive server" << argv [1]<< std::endl;
        if (string(argv[1]) == "report")
        {
            server(io_context, 6688, REPORT_CODE, false);   
        }
        else if(string(argv[1]) == "exam")
        {
            server(io_context, 6688, EXAM_CODE, false);   
        }
        else if(string(argv[1]) == "sendfile")
        {
            server(io_context, 6688, SENDFILE_CODE, false);
        }
    }
    else 
    {
        server(io_context, 6688, 0, true);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

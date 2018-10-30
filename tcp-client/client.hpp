#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <vector>
#include "packet.hpp"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

const int  REGISTER_CODE = 0b10000001; 

class client
{

public:
    client():m_s(m_io_context)
    {}
    void Register(const string& addr, const string& pubK, uint64_t cap, uint64_t avail, uint64_t loc, const string& sig)
    {
        Packet pkt;
        Header header = {1,REGISTER_CODE,0,0,0};
        pkt.setHeader(header);
        pkt.addParam(*(new Param(addr)))
            .addParam(*new Param(pubK))
            .addParam(*new Param(cap))
            .addParam(*new Param(avail))
            .addParam(*new Param(loc))
            .addParam(*new Param(sig));
        pkt.serialize(m_buf);
    }
    void run()
    {
        
        try
        {
            tcp::resolver resolver(m_io_context);
            boost::asio::connect(m_s, resolver.resolve("127.0.0.1", "6688"));
            std::cout << "send message: ";
            std::cout.flush();
            boost::asio::write(m_s, boost::asio::buffer(m_buf));
            char reply[1024] = {0};
            
            
            boost::system::error_code ec;
            size_t reply_length = boost::asio::read(m_s, boost::asio::buffer(reply, 1024),ec);
            
            if (ec) {
                std::cerr << ec << std::endl;
            }
            
            std::cout << "Reply is: ";
            std::cout.flush();
            for(size_t i = 0; i < reply_length; i++)
            {
                std::cout << reply[i];
            }
            
            std::cout.flush();
            std::cout << "\n";
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        
    }
private:
    vector<char> m_buf;
    boost::asio::io_context m_io_context;
    tcp::socket m_s;
};

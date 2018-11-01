#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <vector>
#include "packet.hpp"

using boost::asio::ip::tcp;

enum { max_length = 1024 * 4 };

const int  REGISTER_CODE = 0b10000001; 
const int  REPORT_CODE = 0b00000010;
const int  EXAM_CODE = 0b00000011;
const int  SENDFILE_CODE = 0b00000100;
const int  SAVEFILE_CODE = 0b00000101;
const int  DOWNLOAD_CODE = 0b00000110;
const int  GETBLOCKBEG = 0b00001000;
const int  GETBLOCKEND = 0b00001001;
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
        std::cout << "prepaired to send:" << endl;
        pkt.prettyPrint(cout);
    }

    void Report(uint64_t capcity, uint64_t available, uint64_t location, const string& signature, const string& filelist)
    {
        Packet pkt;
        Header header = {1,REPORT_CODE,0,0,0};
        pkt.setHeader(header);
        pkt.addParam(*(new Param(capcity)))
            .addParam(*new Param(available))
            .addParam(*new Param(location))
            .addParam(*new Param(signature))
            .addParam(*new Param(filelist))
            ;
        pkt.serialize(m_buf);
        std::cout << "prepaired to send:" << endl;
        pkt.prettyPrint(cout);
    }

    void run(bool positiveSession = true)
    {
        
        try
        {
            tcp::resolver resolver(m_io_context);
            boost::asio::connect(m_s, resolver.resolve("127.0.0.1", "6688"));

            
            if (positiveSession)
            {
                boost::asio::write(m_s, boost::asio::buffer(m_buf));
                char reply[max_length] = {0};
                
                boost::system::error_code ec;
                size_t reply_length = boost::asio::read(m_s, boost::asio::buffer(reply, max_length),ec);
                
                if (ec) {
                    std::cerr << ec << std::endl;
                }
                
                std::cout << "Reply is: \n";
                Packet pkt(reply, reply_length);
                pkt.prettyPrint(std::cout);
                std::cout << "\n";
            }
            else
            {   
                char reply[max_length] = {0};
                boost::system::error_code ec;
                size_t reply_length = m_s.read_some(boost::asio::buffer(reply, max_length),ec);
                if (ec) 
                {
                    std::cerr << ec << std::endl;
                }
                
                std::cout << "Revice is: \n";
                Packet pkt(reply, reply_length);
                pkt.prettyPrint(std::cout);

                boost::asio::write(m_s, boost::asio::buffer(m_buf));
            }
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

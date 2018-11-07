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
    void Register(const string& addr, const string& pubK, uint64_t cap, uint64_t avail, uint8_t loc[5], const string& sig)
    {
        Packet pkt;
        Header header = {1,REGISTER_CODE,0,0,0};
        pkt.setHeader(header);
        /* uint8_t loct[5] = {0};
        loct[0] = loc & 0xFF;
        loct[1] = ( loc >> 8 ) & 0xFF;
        loct[2] = ( loc >> 16 ) & 0xFF;
        loct[3] = ( loc >> 24 ) & 0xFF
        loct[4] = ( loc >> 32) & 0xFF; */
        pkt.addParam(Param(addr))
            .addParam(Param(pubK))
            .addParam(Param(cap))
            .addParam(Param(avail))
            .addParam(Param(loc, 5))
            .addParam(Param(sig));
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

    void Exam(uint64_t examser, uint32_t examlen, const string& signature, uint16_t filecount, uint64_t fileid, uint8_t count,  const string& anwser)
    {
        Packet pkt;
        Header header = {1,EXAM_CODE,0,0,0};
        pkt.setHeader(header);
        pkt.addParam(*(new Param(examser)))
            .addParam(*new Param(examlen))
            .addParam(*new Param(signature))
            .addParam(*new Param(filecount))
            .addParam(*new Param(fileid))
            .addParam(*new Param(count))
            .addParam(*new Param(anwser))
            ;
        pkt.serialize(m_buf);
        std::cout << "prepaired to send:" << endl;
        pkt.prettyPrint(cout);
    }

    void SendFile(uint64_t filesize, uint8_t blockcount, uint8_t blocknum, uint8_t blocksize, const string& block, const string& signature)
    {
        Packet pkt;
        Header header = {1,SENDFILE_CODE,0,0,0};
        pkt.setHeader(header);
        pkt.addParam(*(new Param(filesize)))
            .addParam(*new Param(blockcount))
            .addParam(*new Param(blocknum))
            .addParam(*new Param(blocksize))
            .addParam(*new Param(block))
            .addParam(*new Param(signature))
            ;
        pkt.serialize(m_buf);
        std::cout << "prepaired to send: 1 m_buf.size=" << m_buf.size() << endl;
        pkt.prettyPrint(cout);
        /* Packet newPkt(m_buf);
        cout << "!!\n";
        newPkt.prettyPrint(cout); */
    }

    void SaveFile(const string& fileid, const string& verifyId, uint8_t avail, const string& sig)
    {
        Packet pkt;
        Header header = {1,SAVEFILE_CODE,0,0,0};
        pkt.setHeader(header);
        pkt.addParam(*(new Param(fileid)))
            .addParam(*new Param(verifyId))
            .addParam(*new Param(avail))
            .addParam(*new Param(sig))
            ;
        pkt.serialize(m_buf);
        std::cout << "prepaired to send: 1 m_buf.size=" << m_buf.size() << endl;
        pkt.prettyPrint(cout);
        /* Packet newPkt(m_buf);
        cout << "!!\n";
        newPkt.prettyPrint(cout); */
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

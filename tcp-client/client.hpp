#pragma once
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <vector>
#include <boost/bind.hpp>
#include <iostream>
#include "packet.hpp"
using namespace boost::asio;
class client
{
    typedef client this_type;
    typedef ip::tcp::endpoint endpoint_type;
    typedef ip::address address_type;
    typedef ip::tcp::socket socket_type;
    typedef boost::shared_ptr<socket_type> sock_ptr;
    typedef std::vector<char> buffer_type;

private:
    io_service m_io;
    buffer_type m_buf;
    endpoint_type m_ep;

public:
    client():m_buf(100, 0),m_ep(address_type::from_string("127.0.0.1"), 6688)
    {
        start();
    }
    void run()
    {
        m_io.run();
    }
    void prepairData()
    {

    }
    void getData();
    void start()
    {
        sock_ptr sock(new socket_type(m_io));
        sock->async_connect(m_ep, boost::bind(&this_type::conn_handler, this, boost::asio::placeholders::error, sock));
    }
    void conn_handler(const boost::system::error_code& ec, sock_ptr sock)
    {   
        //send data
        if(ec)
        {
            return;
        }
        //std::cout << "connect to " << sock->remote_endpoint().address();
        //sock->async_read_some(buffer(m_buf), boost::bind(&this_type::read_handler, this, boost::asio::placeholders::error));
        
    }
    void read_handler(const boost::system::error_code& ec)
    {
        
        if (ec) {
            return;
        }
        std::cout << &m_buf[0] << std::endl;
        
    }
private:
    void sendData();
};
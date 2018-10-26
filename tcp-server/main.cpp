#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <iostream>
using namespace boost::asio;
using namespace boost;
class server
{
    typedef server this_type;
    typedef ip::tcp::acceptor acceptor_type;
    typedef ip::tcp::endpoint endpoint_type;
    typedef ip::tcp::socket socket_type;
    typedef shared_ptr<socket_type> sock_ptr;

private:
    io_service m_io;
    acceptor_type m_acceptor;

public:
    server() : m_acceptor(m_io, endpoint_type(ip::tcp::v4(), 6688))
    { accept();}
    void run()
    { m_io.run();}
    void accept()
    {
        sock_ptr sock(new socket_type(m_io));
        m_acceptor.async_accept(*sock, boost::bind(&this_type::accept_handler, this, boost::asio::placeholders::error, sock));
    }
    void accept_handler(const system::error_code& ec, sock_ptr sock)
    {
        using namespace std;
        if(ec)
        {
            return;
        }
        cout << "client:";
        cout << sock->remote_endpoint().address() << endl;
        sock->async_write_some(buffer("hello asio"), boost::bind(&this_type::write_handler, this, boost::asio::placeholders::error));
        accept();

    }
    void write_handler(const system::error_code& ec)
    {
        std::cout << "send msg complete." << std::endl;
    }
    void write_handler2(const system::error_code&, std::size_t n)
    {
        std::cout << "send msg " << n << std::endl;
    }
};

int main()
{
    using namespace std;
    try
    {
        cout << "server start. " << endl;
        server srv;
        srv.run();   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

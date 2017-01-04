#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace std;
using namespace boost::asio;

int main()
{
    io_service service;
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8080);
    ip::tcp::socket sock(service);
    sock.open(ip::tcp::v4()); 
    sock.connect(ep);
    sock.write_some(boost::asio::buffer("GET /index.html\r\n"));
    char buff[1024];
    sock.read_some(boost::asio::buffer(buff,1024));
    cout<<buff<<endl;
    sock.shutdown(ip::tcp::socket::shutdown_receive);
    sock.close();
    return 0;
}


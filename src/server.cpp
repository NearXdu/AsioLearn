#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace std;
using namespace boost::asio;

typedef std::shared_ptr<ip::tcp::socket>socket_ptr;

void client_session(socket_ptr sock)
{
    while(true)
    {
	char data[512];
	size_t len=sock->read_some(buffer(data));
	if(len > 0)
	    write(*sock,buffer("ok\r\n",2));
    }
}

int main(int argc,char **argv)
{
    io_service service_;
    ip::tcp::endpoint ep(ip::tcp::v4(),2001);
    ip::tcp::acceptor acc(service_,ep);
    while(true)
    {
	socket_ptr sock(new ip::tcp::socket(service_));
	acc.accept(*sock);
	boost::thread(boost::bind(client_session,sock));
    }
    return 0;
}




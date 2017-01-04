#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace std;
using namespace boost::asio;


typedef std::shared_ptr<ip::udp::socket> sockptr;

void on_read(std::shared_ptr<char>pBuff,sockptr sock,const boost::system::error_code & err, std::size_t read_bytes) {
    std::cout << "read " << read_bytes << std::endl;
    cout<<pBuff;
    pBuff.reset(new char[512]);
    boost::asio::ip::udp::endpoint sender_ep;
    sock->async_receive_from(buffer(pBuff.get(),512), sender_ep, 
	    boost::bind(on_read,pBuff,sock,_1,_2));
}
int main(int argc, char* argv[]) {

    io_service service;

    sockptr psock(new ip::udp::socket(service));
    ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"),
	    1024);
    psock->open(ep.protocol());
    psock->set_option(boost::asio::ip::udp::socket::reuse_address(true));
    psock->bind(ep);

    char* buff=new char[512];
    std::shared_ptr<char>pBuff(buff);

    boost::asio::ip::udp::endpoint sender_ep;
    psock->async_receive_from(buffer(pBuff.get(),512), sender_ep, 
	    boost::bind(on_read,pBuff,psock,_1,_2));


    service.run();
}

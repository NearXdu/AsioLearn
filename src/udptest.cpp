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
    ip::udp::socket sock(service);
    sock.open(ip::udp::v4());
    ip::udp::endpoint receiver_ep(ip::address::from_string("127.0.0.1"), 8089);//echo
    sock.send_to(boost::asio::buffer("testing---echo"), receiver_ep);
    char buff[512];
    ip::udp::endpoint sender_ep;
    //receive_from从udp套接字读取数据时，需要构造一个默认端点
  //  sock.receive_from(boost::asio::buffer(buff), sender_ep);
//    cout<<buff<<endl;
    return 0;
}


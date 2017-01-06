/*
 *This tutorial program shows how to use asio to implement a client application with UDP.
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;

int main(int argc,char**argv)
{
    try
    {
	if(argc!=2)
	{
	    std::cerr<<"Usage : client <host>"<<std::endl;
	    return 1;
	}
	boost::asio::io_service io_service;
	ip::udp::resolver resolver(io_service);
	ip::udp::resolver::query query(ip::udp::v4(),argv[1],"daytime");

	//This means it is safe to dereference the return value directly.
	ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

	ip::udp::socket socket(io_service);
	socket.open(ip::udp::v4());
	boost::array<char, 1> send_buf  = {{ 0 }};
	socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

	boost::array<char, 128> recv_buf;
	ip::udp::endpoint sender_endpoint;//local endpoint
	size_t len = socket.receive_from(
		boost::asio::buffer(recv_buf), sender_endpoint);

	std::cout.write(recv_buf.data(), len);
    }
    catch(std::exception &e)
    {
	std::cerr<<e.what()<<std::endl;
    }

}

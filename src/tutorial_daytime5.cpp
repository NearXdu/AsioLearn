//This tutorial program shows how to use asio to implement a server application with UDP.
//
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;

std::string make_daytime_string()
{
    using namespace std;// For time_t, time and ctime;
    time_t now =time(0);
    return ctime(&now);
}

int main()
{
    try
    {
	boost::asio::io_service io_service;
	ip::udp::socket socket(io_service, ip::udp::endpoint(udp::v4(), 8813));


	for (;;)
	{
	    boost::array<char, 1> recv_buf;
	    ip::udp::endpoint remote_endpoint;
	    boost::system::error_code error;
	    socket.receive_from(boost::asio::buffer(recv_buf),
		    remote_endpoint, 0, error);

	    if (error && error != boost::asio::error::message_size)
		throw boost::system::system_error(error);

	    std::string message = make_daytime_string();

	    boost::system::error_code ignored_error;
	    socket.send_to(boost::asio::buffer(message),
		    remote_endpoint, 0, ignored_error);
	}

    }
    catch(std::exception &e)
    {

	std::cerr << e.what() << std::endl;
    }
    return 0;
}

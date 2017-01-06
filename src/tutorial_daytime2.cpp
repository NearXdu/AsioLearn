/*
 * This tutorial program shows how to 
 * use asio to implement a server application with TCP.
 */

#include <iostream>
#include <boost/asio.hpp>
#include <ctime>
#include <string>

using namespace boost::asio;


/*
 *We define the function make_daytime_string() 
 *to create the string to be sent back to the client. 
 *This function will be reused in all of our daytime server applications.
 */

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

	//acceptor is created to listen connection
	ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), 8813));//port 8813

	//This is an iterative server, which means that it will handle one connection at a time

	for(;;)
	{
	    ip::tcp::socket socket(io_service);
	    acceptor.accept(socket);

	    std::string message = make_daytime_string();

	    boost::system::error_code ignored_error;
	    boost::asio::write(socket, boost::asio::buffer(message.c_str(),message.size()), ignored_error);
	}
    }
    catch(std::exception &e)
    {
	std::cerr<<e.what()<<std::endl;
    }
    return 0;


}

/*
 *An asynchronous UDP daytime server
 */

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}
class udp_server
{
    public:
	/*Construct*/
	udp_server(boost::asio::io_service &io_service)
	    :socket_(io_service,udp::endpoint(udp::v4(),8813))
	{
	    start_receive();
	}

    private:

	void start_receive()
	{
	    socket_.async_receive_from(boost::asio::buffer(recv_buffer_),remote_endpoint_,
		    boost::bind(&udp_server::handle_receive,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void handle_receive(const boost::system::error_code&error,
		std::size_t bytes_transferred)
	{
	    if (!error || error == boost::asio::error::message_size)
	    {
//		std::shared_ptr<std::string>message(new std::string(make_daytime_string()));

		std::string message = make_daytime_string();

		socket_.async_send_to(boost::asio::buffer(message), remote_endpoint_,
			boost::bind(&udp_server::handle_send, this,
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred));

		start_receive();
	    }
	}
	void handle_send(const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}

#if 0
	void handle_send(std::shared_ptr<std::string> /*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}
#endif
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<char,1>recv_buffer_;

};
int main()
{
    try
    {
	boost::asio::io_service io_service;
	udp_server server(io_service);

	io_service.run();
    }
    catch(std::exception &e)
    {
	std::cerr<<e.what()<<std::endl;
    }
    return 0;
}

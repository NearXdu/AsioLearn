/*
 *An asynchronous TCP daytime server
 */

#include <iostream>
#include <boost/asio.hpp>
#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <memory>//shared_ptr
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::asio;

std::string make_daytime_string()
{
    using namespace std;// For time_t, time and ctime;
    time_t now =time(0);
    return ctime(&now);
}

class tcp_connection:
    public std::enable_shared_from_this<tcp_connection>
{
    public:
	typedef std::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_service& io_service)
	{
	    return pointer(new tcp_connection(io_service));//shared_ptr handle connection
	}
	ip::tcp::socket&socket()
	{
	    return socket_;
	}
	void start()
	{
	    message_=make_daytime_string();
	    boost::asio::async_write(socket_,boost::asio::buffer(message_),
		    boost::bind(&tcp_connection::handle_write,
			shared_from_this(),//ensure the life tiem of the obj
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

    private:
	tcp_connection(boost::asio::io_service& io_service)
	    : socket_(io_service)
	{
	}
	void handle_write(const boost::system::error_code& /*error*/,
		size_t /*bytes_transferred*/)
	{
	}

	ip::tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
    public:
	tcp_server(boost::asio::io_service& io_service)
	    :acceptor_(io_service,
		    ip::tcp::endpoint(ip::tcp::v4(),8813))
    {
	start_accept();
    }

    private:

	void start_accept()
	{
	    tcp_connection::pointer new_connection =
		tcp_connection::create(acceptor_.get_io_service());

	    acceptor_.async_accept(new_connection->socket(),
		    boost::bind(&tcp_server::handle_accept, 
			this,
		       	new_connection,
			boost::asio::placeholders::error));

	}

	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error)
	{
	    if (!error)
	    {
		new_connection->start();
	    }

	    start_accept();//listen another connection
	}


	ip::tcp::acceptor acceptor_; //listen connection

};

int main()
{
    try
    {
	boost::asio::io_service io_service;
	/**/
	tcp_server server(io_service);
	io_service.run();//perform async operation

    }
    catch(std::exception &e)
    {
	std::cerr<<e.what()<<std::endl;
    }

    return 0;
}

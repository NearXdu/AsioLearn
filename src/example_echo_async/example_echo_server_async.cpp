#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

class session
  : public std::enable_shared_from_this<session>
{
    public:
	session(tcp::socket socket):socket_(std::move(socket))
    {}
	void start()
	{
	    do_read();
	}
    private:
	void do_read()
	{
	    auto self(shared_from_this());
	    socket_.async_read_some(boost::asio::buffer(data_,max_length),
		    [self,this](boost::system::error_code ec, std::size_t length)
		    {
		    	if(!ec)
			do_write(length);
		    });
	}
	void do_write(std::size_t length)
	{
	    auto self(shared_from_this());
	    boost::asio::async_write(socket_,boost::asio::buffer(data_,length),
		    [this, self](boost::system::error_code ec, std::size_t /*length*/)
		    {
		    	if (!ec)
		    	{
		    		do_read();
		    	}
		    });

	}
	enum {max_length=1024};
	char data_[max_length];
	tcp::socket socket_;
};
class server
{
    public:
	server(boost::asio::io_service & io_service,unsigned short port)
	    :acceptor_(io_service,tcp::endpoint(tcp::v4(),port)),socket_(io_service)
	{
	    do_accept();
	}

    private:

	void do_accept()
	{
	    acceptor_.async_accept(socket_,
		    [this](boost::system::error_code ec)
		    {
		    	if(!ec)
		    	{
		    		std::make_shared<session>(std::move(socket_))->start();
		    	}

		    	do_accept();
		    });//异步接收连接
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
};//server


int main(int argc, char* argv[])
{

    try
    {
#if 0
	if (argc != 2)
	{
	    std::cerr << "Usage: async_tcp_echo_server <port>\n";
	    return 1;
	}
#endif

	boost::asio::io_service io_service;

	server s(io_service, 1024);

	io_service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}


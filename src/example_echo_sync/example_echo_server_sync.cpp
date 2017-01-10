#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using namespace boost::asio::ip;
const int max_length = 1024;
void session(boost::asio::ip::tcp::socket sock)
{
    try
    {
	for (;;)
	{
	    char data[max_length];

	    boost::system::error_code error;
	    size_t length = sock.read_some(boost::asio::buffer(data), error);//阻塞等待读取消息
	    if (error == boost::asio::error::eof)
		break; // Connection closed cleanly by peer.
	    else if (error)
		throw boost::system::system_error(error); // Some other error.

	    boost::asio::write(sock, boost::asio::buffer(data, length));//向网络套接字写消息
	}
    }
    catch (std::exception& e)
    {
	std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}


void server(boost::asio::io_service &io_service, unsigned short port)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));//初始化监听套接字
    for (;;)//循环监听
    {
	tcp::socket sock(io_service);//创建已连接套接子
	a.accept(sock);
	//set detach来接收新的新的连接而不是阻塞join
	std::thread(session, std::move(sock)).detach();//one-thread per connection
    }
}
int main()
{
    try
    {
#if 0
	if (argc != 2)
	{
	    std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
	    return 1;
	}
#endif

	boost::asio::io_service io_service;

	server(io_service, 1024);
    }
    catch (std::exception& e)
    {
	std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

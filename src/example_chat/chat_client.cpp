#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
    public:
	chat_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
	    : io_service_(io_service),
	    socket_(io_service)//初始化
    {
	do_connect(endpoint_iterator);
    }

	void write(const chat_message& msg)
	{
	    io_service_.post(//发起异步事件
		    [this, msg]()
		    {
		    bool write_in_progress = !write_msgs_.empty();
		    write_msgs_.push_back(msg);
		    if (!write_in_progress)
		    {
		    do_write();
		    }
		    });
	}

	void close()
	{
	    io_service_.post([this]() { socket_.close(); });
	}

    private:
	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
	    //发起连接
	    boost::asio::async_connect(socket_, endpoint_iterator,
		    [this](boost::system::error_code ec, tcp::resolver::iterator)
		    {
		    if (!ec)
		    {
		    do_read_header();//读取消息
		    }
		    });
	}

	void do_read_header()
	{
	    boost::asio::async_read(socket_,
		    boost::asio::buffer(read_msg_.data(), chat_message::header_length),
		    [this](boost::system::error_code ec, std::size_t /*length*/)
		    {
		    if (!ec && read_msg_.decode_header())
		    {
		    do_read_body();
		    }
		    else
		    {
		    socket_.close();
		    }
		    });
	}

	void do_read_body()
	{
	    boost::asio::async_read(socket_,
		    boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
		    [this](boost::system::error_code ec, std::size_t /*length*/)
		    {
		    if (!ec)
		    {
		    std::cout.write(read_msg_.body(), read_msg_.body_length());
		    std::cout << "\n";//打印消息到屏幕
		    do_read_header();
		    }
		    else
		    {
		    socket_.close();
		    }
		    });
	}

	void do_write()
	{
	    boost::asio::async_write(socket_,
		    boost::asio::buffer(write_msgs_.front().data(),
			write_msgs_.front().length()),
		    [this](boost::system::error_code ec, std::size_t /*length*/)
		    {
		    if (!ec)
		    {
		    write_msgs_.pop_front();
		    if (!write_msgs_.empty())
		    {
		    do_write();
		    }
		    }
		    else
		    {
		    socket_.close();
		    }
		    });
	}

    private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
    //客户端需要两个线程
    //主线程负责读取标准输入
    //thread线程负责调度异步事件(io_service.run())
    try
    {
#if 0
	if (argc != 3)
	{
	    std::cerr << "Usage: chat_client <host> <port>\n";
	    return 1;
	}
#endif

	boost::asio::io_service io_service;

	tcp::resolver resolver(io_service);
	auto endpoint_iterator = resolver.resolve({ "localhost", "1024" });
	chat_client c(io_service, endpoint_iterator);//客户端

	std::thread t([&io_service](){ io_service.run(); });//启动线程执行io_service.run()


	char line[chat_message::max_body_length + 1];
	while (std::cin.getline(line, chat_message::max_body_length + 1))
	{
	    chat_message msg;
	    msg.body_length(std::strlen(line));//获取header信息
	    std::memcpy(msg.body(), line, msg.body_length());
	    msg.encode_header();//消息编码
	    c.write(msg);
	}

	c.close();
	t.join();
    }
    catch (std::exception& e)
    {
	std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
using namespace std;
using namespace boost::asio;


class udpserver
{
    public:
	udpserver(io_service & service):socket_(service,ip::udp::endpoint(ip::udp::v4(),1024))
    {
	start_receive();
    }

    private:
	void start_receive()
	{
	    socket_.async_receive_from(buffer(recv_buffer_,max_length),
		    remote_endpoint_,
		    boost::bind(&udpserver::handle_receive,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_send(std::shared_ptr<std::string> message,
		const boost::system::error_code& error,
		std::size_t bytes_transferred)
	{
	    cout<<"received_buffer:"<<endl;
	    cout<<*message;
	}
	

	void handle_receive(const boost::system::error_code& error,
		std::size_t bytes_transferred)
	{
	    if (!error || error == boost::asio::error::message_size)
	    {
		if(bytes_transferred>0)
		{
		    std::string*ptr_string= new std::string(recv_buffer_,recv_buffer_+bytes_transferred);
		    std::shared_ptr<std::string> message(ptr_string);
		    socket_.async_send_to(boost::asio::buffer(recv_buffer_,bytes_transferred), remote_endpoint_,
			    boost::bind(&udpserver::handle_send, this, message,
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred));

		    start_receive();
		}
	    }
	}
	ip::udp::socket socket_;
	ip::udp::endpoint remote_endpoint_;
	enum {max_length=1024};
	char recv_buffer_[max_length];
//	boost::array<char,1024>recv_buffer_;

};

int main()
{
    try
    {
	io_service service;
	udpserver server(service);
	service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << e.what() << std::endl;
    }
    return 0;
}


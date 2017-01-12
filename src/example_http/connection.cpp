#include "connection.hpp"
#include <utility>
#include <vector>
#include "connection_manager.hpp"
#include "request_handler.hpp"

namespace http {
    namespace server {

	connection::connection(boost::asio::ip::tcp::socket socket,
		connection_manager& manager, request_handler& handler)
	    : socket_(std::move(socket)),
	    connection_manager_(manager),
	    request_handler_(handler)
	{
	}

	void connection::start()
	{
	    do_read();//异步读取数据
	}

	void connection::stop()
	{
	    socket_.close();
	}

	void connection::do_read()
	{
	    auto self(shared_from_this());
	    socket_.async_read_some(boost::asio::buffer(buffer_),
		    [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
		    {
		    //完成回调，解析HTTP请求
		    if (!ec)
		    {
		    request_parser::result_type result;
		    std::tie(result, std::ignore) = request_parser_.parse(
			request_, buffer_.data(), buffer_.data() + bytes_transferred);//request,iterator_begin(),iterator_end()

		    if (result == request_parser::good)//解析请求成功
		    {
		    request_handler_.handle_request(request_, reply_);
		    do_write();
		    }
		    else if (result == request_parser::bad)//解析请求失败
		    {
		    reply_ = reply::stock_reply(reply::bad_request);
		    do_write();
		    }
		    else//继续异步读取消息
		    {
			do_read();
		    }
		    }
		    else if (ec != boost::asio::error::operation_aborted)
		    {
			connection_manager_.stop(shared_from_this());
		    }
		    });
	}

	void connection::do_write()
	{
	    auto self(shared_from_this());
	    boost::asio::async_write(socket_, reply_.to_buffers(),
		    [this, self](boost::system::error_code ec, std::size_t)
		    {
		    if (!ec)
		    {
		    // Initiate graceful connection closure.
		    boost::system::error_code ignored_ec;
		    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
			ignored_ec);
		    }

		    if (ec != boost::asio::error::operation_aborted)
		    {
		    connection_manager_.stop(shared_from_this());
		    }
		    });
	}

    } // namespace server
} // namespace http

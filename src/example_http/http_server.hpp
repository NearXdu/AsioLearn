#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__
#include <boost/asio.hpp>
#include <string>
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"

namespace http {
    namespace server {

	/// The top-level class of the HTTP server.
	class server
	{
	    public:
		server(const server&) = delete;
		server& operator=(const server&) = delete;

		/// Construct the server to listen on the specified TCP address and port, and
		/// serve up files from the given directory.
		explicit server(const std::string& address, const std::string& port,
			const std::string& doc_root);

		/// Run the server's io_service loop.
		void run();

	    private:
		/// Perform an asynchronous accept operation.
		void do_accept();//异步接收新连接

		/// Wait for a request to stop the server.
		void do_await_stop();//异步等待信号

		/// The io_service used to perform asynchronous operations.
		boost::asio::io_service io_service_;//执行异步操作

		/// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals_;//注册中止消息

		/// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor_;//监听套接字

		/// The connection manager which owns all live connections.
		connection_manager connection_manager_;

		/// The next socket to be accepted.
		boost::asio::ip::tcp::socket socket_;//已连接套接字

		/// The handler for all incoming requests.
		request_handler request_handler_;
	};

    } // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP

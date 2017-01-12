#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <string>

namespace http {
    namespace server {

	struct reply;
	struct request;

	/// The common handler for all incoming requests.
	class request_handler
	{
	    public:
		//禁止拷贝构造函数和赋值重载
		request_handler(const request_handler&) = delete;
		request_handler& operator=(const request_handler&) = delete;

		/// Construct with a directory containing files to be served.
		explicit request_handler(const std::string& doc_root);

		/// Handle a request and produce a reply.
		void handle_request(const request& req, reply& rep);

	    private:
		/// The directory containing the files to be served.
		std::string doc_root_;//文件目录

		/// Perform URL-decoding on a string. Returns false if the encoding was
		/// invalid.
		static bool url_decode(const std::string& in, std::string& out);//URL分析
	};

    } // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP

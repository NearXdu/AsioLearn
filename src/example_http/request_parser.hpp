#ifndef __REQUEST_PARSER_HPP__
#define __REQUEST_PARSER_HPP__
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <tuple>

namespace http {
    namespace server {

	struct request;

	/// Parser for incoming requests.
	class request_parser
	{
	    public:
		/// Construct ready to parse the request method.
		request_parser();

		/// Reset to initial parser state.
		void reset();

		/// Result of parse.
		enum result_type { good, bad, indeterminate };

		/// Parse some data. The enum return value is good when a complete request has
		/// been parsed, bad if the data is invalid, indeterminate when more data is
		/// required. The InputIterator return value indicates how much of the input
		/// has been consumed.
		template <typename InputIterator>
		    std::tuple<result_type, InputIterator> parse(request& req,
			    InputIterator begin, InputIterator end)
		    {
			while (begin != end)
			{
			    result_type result = consume(req, *begin++);
			    if (result == good || result == bad)
				return std::make_tuple(result, begin);
			}
			return std::make_tuple(indeterminate, begin);
		    }

	    private:
		/// Handle the next character of input.
		result_type consume(request& req, char input);

		/// Check if a byte is an HTTP character.
		static bool is_char(int c);

		/// Check if a byte is an HTTP control character.
		static bool is_ctl(int c);

		/// Check if a byte is defined as an HTTP tspecial character.
		static bool is_tspecial(int c);

		/// Check if a byte is a digit.
		static bool is_digit(int c);

		/// The current state of the parser.
		//解析http请求状态
		enum state
		{
		    method_start,//方法开始
		    method,//请求方法
		    uri,//请求uri
		    http_version_h,//h
		    http_version_t_1,//t
		    http_version_t_2,//t
		    http_version_p,//p
		    http_version_slash,//斜杠
		    http_version_major_start,//
		    http_version_major,//主版本号
		    http_version_minor_start,
		    http_version_minor,//次版本号
		    expecting_newline_1,//\r\n
		    ////////////////////////////////
		    header_line_start,//请求头
		    header_lws,//
		    header_name,//首部key
		    space_before_header_value,//之间的空行
		    header_value,//首部value
		    expecting_newline_2,
		    expecting_newline_3
		} state_;
	};

    } // namespace server
} // namespace http

#endif // HTTP_REQUEST_PARSER_HPP
#endif

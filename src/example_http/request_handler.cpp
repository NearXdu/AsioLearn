#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace http {
    namespace server {

	request_handler::request_handler(const std::string& doc_root)
	    : doc_root_(doc_root)
	{
	}//构造函数

	void request_handler::handle_request(const request& req, reply& rep)
	{//解析请求
	    // Decode url to path.
	    std::string request_path;
	    if (!url_decode(req.uri, request_path))//解析请求地址
	    {
		rep = reply::stock_reply(reply::bad_request);
		return;
	    }//解析url

	    // Request path must be absolute and not contain "..".
	    // 请求url的条件
	    if (request_path.empty() || request_path[0] != '/'
		    || request_path.find("..") != std::string::npos)
	    {
		rep = reply::stock_reply(reply::bad_request);
		return;
	    }

	    // If path ends in slash (i.e. is a directory) then add "index.html".
	    if (request_path[request_path.size() - 1] == '/')//如果请求url最后一个字符是/，那么加上一个index.html
	    {
		request_path += "index.html";
	    }

	    // Determine the file extension.
	    std::size_t last_slash_pos = request_path.find_last_of("/");//最后一个/符号
	    std::size_t last_dot_pos = request_path.find_last_of(".");//最后一个.
	    std::string extension;
	    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
	    {
		extension = request_path.substr(last_dot_pos + 1);//获得扩展名
	    }

	    // Open the file to send back.
	    std::string full_path = doc_root_ + request_path;//文件的完整目录
	    std::cout<<full_path<<std::endl;
	    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);//打开文件
	    if (!is)
	    {
		rep = reply::stock_reply(reply::not_found);
		return;
	    }

	    // Fill out the reply to be sent to the client.
	    // 响应码
	    rep.status = reply::ok;//response
	    char buf[512];
	    while (is.read(buf, sizeof(buf)).gcount() > 0)
	    {
		//append(const char *,size_t size)
		rep.content.append(buf, is.gcount());//gcount()返回读取数目
	    }
	    //响应头
	    rep.headers.resize(2);
	    rep.headers[0].name = "Content-Length";
	    rep.headers[0].value = std::to_string(rep.content.size());
	    rep.headers[1].name = "Content-Type";
	    rep.headers[1].value = mime_types::extension_to_type(extension);//扩展名->Content-Type
	}

	bool request_handler::url_decode(const std::string& in, std::string& out)
	{
	    out.clear();
	    out.reserve(in.size());//
	    for (std::size_t i = 0; i < in.size(); ++i)
	    {
		if (in[i] == '%')//转义字符
		{
		    if (i + 3 <= in.size())
		    {
			int value = 0;
			std::istringstream is(in.substr(i + 1, 2));
			if (is >> std::hex >> value)//16进制转10进制（0-255）
			{
			    out += static_cast<char>(value);
			    i += 2;
			}
			else
			{
			    return false;
			}
		    }
		    else
		    {
			return false;
		    }
		}
		else if (in[i] == '+')//+表示空格
		{
		    out += ' ';
		}
		else//如果非特殊符号直接
		{
		    out += in[i];
		}
	    }
	    return true;
	}//解析url

    } // namespace server
} // namespace http

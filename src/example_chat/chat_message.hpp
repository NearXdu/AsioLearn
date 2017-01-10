#ifndef __CHAT_MESSAGE_HPP__
#define __CHAT_MESSAGE_HPP__
#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
    public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	chat_message():body_length_(0)
    {
    }
	//get method
	const char* data() const
	{
	    return data_;
	}

	char* data()
	{
	    return data_;
	}

	std::size_t length() const
	{
	    //header length is fixed
	    return header_length + body_length_;
	}

	//get body data
	const char* body() const
	{
	    //body data
	    return data_ + header_length;
	}

	char* body()
	{
	    return data_ + header_length;
	}

	//get 
	std::size_t body_length() const
	{
	    return body_length_;
	}

	//set
	void body_length(std::size_t new_length)
	{
	    body_length_ = new_length;
	    if (body_length_ > max_body_length)
		body_length_ = max_body_length;
	}

	//header decode
	bool decode_header()
	{
	    char header[header_length + 1] = "";
	    std::strncat(header, data_, header_length);//get header
	    body_length_ = std::atoi(header);//header is info of body_length
	    if (body_length_ > max_body_length)
	    {
		body_length_ = 0;
		return false;
	    }
	    return true;
	}

	//message encode
	void encode_header()
	{
	    char header[header_length + 1] = "";
	    std::sprintf(header, "%4d", static_cast<int>(body_length_));//4 bytes header
	    std::memcpy(data_, header, header_length);
	}

    private:
	char data_[header_length + max_body_length];//512+4
	std::size_t body_length_;//body length

};
#endif

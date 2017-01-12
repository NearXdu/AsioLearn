#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include <string>

namespace http {
    namespace server {

	struct header
	{
	    std::string name;
	    std::string value;
	};

    } // namespace server
} // namespace http

#endif // HTTP_HEADER_HPP

#include "connection_manager.hpp"

namespace http {
    namespace server {

	connection_manager::connection_manager()
	{
	}

	void connection_manager::start(connection_ptr c)
	{
	    connections_.insert(c);//connections_为保存connection的shared_ptr set
	    c->start();//调用connection的start开始读取网络套接字数据
	}

	void connection_manager::stop(connection_ptr c)
	{
	    connections_.erase(c);
	    c->stop();
	}

	void connection_manager::stop_all()
	{
	    for (auto c: connections_)
		c->stop();
	    connections_.clear();
	}

    } // namespace server
} // namespace http

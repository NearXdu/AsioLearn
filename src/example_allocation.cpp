/*
 *This example shows how to customise the allocation of memory associated with asynchronous operations.
 */
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
// 
// 包含一个栈上空间，以满足请求的存储分配

// 当空间不够，或者栈上空间在使用，则在堆中分配空间

class session;
std::weak_ptr<session> weakPtr;
class handler_allocator
{
public:
  handler_allocator()
    : in_use_(false)
  {
  }

  //拷贝构造函数和赋值操作重载被禁止
  handler_allocator(const handler_allocator&) = delete;
  handler_allocator& operator=(const handler_allocator&) = delete;

  void* allocate(std::size_t size)
  {
    if (!in_use_ && size < sizeof(storage_))//sizeof(storagte_)==1024
    {
      in_use_ = true;
      return &storage_;
    }
    else//申请更大的空间
    {
      return ::operator new(size);
    }
  }

  void deallocate(void* pointer)
  {
    if (pointer == &storage_)//释放空间
    {
      in_use_ = false;
    }
    else//delelte
    {
      ::operator delete(pointer);
    }
  }

private:
  // Storage space used for handler-based custom memory allocation.
  typename std::aligned_storage<1024>::type storage_;

  // Whether the handler-based custom allocation storage has been used.
  bool in_use_;
};//class handler_allocator

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <typename Handler>
class custom_alloc_handler
{
public:
  custom_alloc_handler(handler_allocator& a, Handler h)
    : allocator_(a),
      handler_(h)
  {
  }
#if 1
  template <typename ...Args>
  void operator()(Args&&... args)
  {
      handler_(std::forward<Args>(args)...);//调用handler
  }
#endif
  

  friend void* asio_handler_allocate(std::size_t size,
      custom_alloc_handler<Handler>* this_handler)
  {
    return this_handler->allocator_.allocate(size);
  }

  friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/,
      custom_alloc_handler<Handler>* this_handler)
  {
    this_handler->allocator_.deallocate(pointer);
  }

private:
  handler_allocator& allocator_;
  Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(
	handler_allocator& a, Handler h)
{
    return custom_alloc_handler<Handler>(a, h);
}
class session:public std::enable_shared_from_this<session>
{
    public:
	session(tcp::socket socket_)
	    :socket_(std::move(socket_))//构造函数
	{
	}
	void start()
	{
	    do_read();
	}
    private:
	void do_read()
	{
	    auto self(shared_from_this());
	    /*
	     *void async_read_some(
	     *const MutableBufferSequence & buffers,
	     *ReadHandler handler);
	     */ 
#if 1
	    auto handler=make_custom_alloc_handler(allocator_,
			[this,self](boost::system::error_code ec,std::size_t length)
			{
				if(!ec)
				{
					std::cout<<"read: "<<length<<"bytes"<<std::endl;
					do_read();
				}
			});
#endif

	    socket_.async_read_some(boost::asio::buffer(data_),
		    handler);
	}
	tcp::socket socket_;
	std::array<char,1024>data_;

	handler_allocator allocator_;

};


class server
{
    public:
	//constructor
	server(boost::asio::io_service & io_service,short port)
	    :acceptor_(io_service,tcp::endpoint(tcp::v4(),port)),
	    socket_(io_service)//构造函数
    {
	do_accept();//开始监听
    }
    private:
	void do_accept()
	{
	    acceptor_.async_accept(socket_,
		    [this](boost::system::error_code ec)//lambda
		    {
		    if(!ec) 
		    {
		    //std::move
		    auto ptr_= std::make_shared<session>(std::move(socket_));//shared_ptr
		    weakPtr=ptr_;
		    ptr_->start();
		    }
		    do_accept();
		    });

	}



	tcp::acceptor acceptor_;
	tcp::socket socket_;
};//class server

int main(int argc,char **argv)
{
    try
    {
#if  0
	if(2!=argc)
	{
	    std::cerr<<"Usage: server <port>"<<std::endl;
	    return 1;
	}
#endif
	boost::asio::io_service io_service;
	//	server s(io_service,std::atoi(argv[1])); //1.初始化
	server s(io_service,9981); //1.初始化

	io_service.run();
    }
    catch(std::exception&e)
    {
	std::cerr<<e.what()<<std::endl;
    }

}

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

class printer
{
    public:
	printer(boost::asio::io_service& io)
	    : strand_(io),
	    //
	    timer1_(io, boost::posix_time::seconds(1)),
	    timer2_(io, boost::posix_time::seconds(1)),
	    count_(0)
    {
		timer1_.async_wait(boost::bind(&printer::print1, this));
		timer2_.async_wait(boost::bind(&printer::print2, this));
//	timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
//	timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));

    }

	~printer()
	{
	    std::cout << "Final count is " << count_ << std::endl;
	}

	void print1()
	{
	    if (count_ < 10)
	    {

		//std::cout << "Timer 1: " << count_ << std::endl;
		std::cout<<"Timer 1 "<<"myThreadId is "<<getThreadId()<<std::endl;
		++count_;

		timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
		timer1_.async_wait(boost::bind(&printer::print1, this));

	//	timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
	    }
	}

	void print2()
	{
	    if (count_ < 10)
	    {
		//std::cout << "Timer 2: " << count_ << std::endl;

		std::cout<<"Timer 2 "<<"myThreadId is "<<getThreadId()<<std::endl;
		++count_;

		timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));

			timer2_.async_wait(boost::bind(&printer::print2, this));
	//	timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
	    }
	}

    private:
	unsigned long getThreadId()
	{
	    std::string threadId=boost::lexical_cast<std::string>(boost::this_thread::get_id());
	    unsigned long  threadNumber=0;
	    threadNumber =std::stoul(threadId,nullptr,16);
//	    sscanf(threadId.c_str(), "%lx", &threadNumber);
	    return threadNumber;
	}
	boost::asio::io_service::strand strand_;
	//两个timer
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;

	int count_;
};

int main()
{
    boost::asio::io_service io;
    printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io));//thread1
    io.run();//main thread
    t.join();

    return 0;
}

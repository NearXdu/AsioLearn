#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using namespace std;
using namespace boost::asio;


int main(int argc,char **argv)
{
    if(argc <2)
    {
	cout<<"usage: ./dnstest [hostname]"<<endl;
	return -1;
    }
    io_service service_;
    ip::tcp::resolver resolver_(service_);
    ip::tcp::resolver::query query_(argv[1],"80");
    ip::tcp::resolver::iterator iter=resolver_.resolve(query_);
    ip::tcp::resolver::iterator end;
    while(iter!=end)
    {
	ip::tcp::endpoint ep=*iter;
	cout<<"---->"<<ep.address().to_string()<<endl;
	++iter;
    }
    return 0;
}




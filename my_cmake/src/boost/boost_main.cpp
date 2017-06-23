#include <boost/asio.hpp>
#include <boost/thread.hpp> 
#include <iostream> 

boost::asio::io_service io_service_1;
	boost::asio::io_service io_service_2;
	
void handler1(const boost::system::error_code &ec) {
	std::cout << "5 seconds." << std::endl;
}

void handler2(const boost::system::error_code &ec) {
	std::cout << "10 seconds." << std::endl;
}

void thread_func_1() {
	std::cout << "## first thread ##" << std::endl;
	io_service_1.run();
}

void thread_func_2() {
	std::cout << "## second thread ##" << std::endl;
	io_service_2.run();
}

int main() {
	//1-boost:asio time handle
//	boost::asio::io_service io_service;
//	boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5));
//	timer1.async_wait(handler1);
//	boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(10));
//	timer2.async_wait(handler2);
//	io_service.run();
	
	//2-boost-asio thread
	boost::asio::io_service io_service_1;
	boost::asio::io_service io_service_2;
	boost::asio::deadline_timer timer1(io_service_1, boost::posix_time::seconds(5));
	timer1.async_wait(handler1);
	boost::asio::deadline_timer timer2(io_service_2, boost::posix_time::seconds(5));
	timer2.async_wait(handler2);
	boost::thread thread1.run(thread_func_1);
	boost::thread thread2.run(thread_func_2);
	thread1.join();
	thread2.join();
}


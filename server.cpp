#include <iostream>

#include <boost/asio.hpp>


std::string read_(boost::asio::ip::tcp::socket& socket) {
	std::cout << "in read_\n";
	boost::asio::streambuf buf;
	std::cout << "created buffer\n";
	buf.prepare(1000000);
	std::cout << "redserved space\n";
	boost::asio::read_until(socket, buf , '\n');
	//socket.receive(buf);
	std::cout << "socket read\n";
	std::string data = boost::asio::buffer_cast<const char*>(buf.data());
	return data;
}
void send_(boost::asio::ip::tcp::socket& socket, const std::string& message) {
	const std::string msg = message + "\n";
	boost::asio::write(socket, boost::asio::buffer(message));
}

int main(int argc, char** argv)
{
	system("chcp 1251");

	using namespace boost::asio;
	//--------------------------------------------------------------------------
	auto port = 3333;
	//ip::address ip_address = boost::asio::ip::address_v6::any();
	
	std::string raw_ip_address = "127.0.0.1";
	
	boost::system::error_code error_code;
	ip::address ip_address =ip::address::from_string(raw_ip_address, error_code);
	ip::tcp::endpoint endpoint_(ip_address, port);
	std::cout << "Endpoint ready" << std::endl;

	io_service io_service_;
	ip::tcp protocol_ = boost::asio::ip::tcp::v4();
	//--------------------------------------------------------------------------
	try
	{	
		ip::tcp::acceptor acceptor_(io_service_, endpoint_);
		std::cout << "acceptor created\n";
		ip::tcp::socket socket_(io_service_/*, protocol_*/);
		std::cout << "socket created\n";

	//--------------------------------------------------------------------------
		acceptor_.accept(socket_);
		std::cout << "Socket ready" << std::endl;
	//--------------------------------------------------------------------------
		std::cout << "reading\n";
		std::string message = read_(socket_);
		std::cout << message << std::endl;

		send_(socket_, "Hello From Server!");
		std::cout << "Servent sent Hello message to Client!" << std::endl;

	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
	}

	
	//--------------------------------------------------------------------------

	

	system("pause");

	return EXIT_SUCCESS;
}

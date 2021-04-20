#include <iostream>
#include <string>
#include <boost/asio.hpp>


std::string read_(boost::asio::ip::tcp::socket& socket) {
	/*boost::asio::streambuf buffer;
	boost::asio::read_until(socket, buffer, '!');
	std::string message;
	std::istream input_stream(&buffer);
	std::getline(input_stream, message, '\n');
	return message;*/


	const std::size_t length = 10;
	char buffer[length];
	boost::asio::read(socket, boost::asio::buffer(buffer, length));
	return std::string(buffer, length);
}
void send_(boost::asio::ip::tcp::socket& socket, const std::string& message) {
	std::cout << " in send_\n";
	//boost::asio::write(socket, boost::asio::buffer(message));
	
	std::string data = "Hello from server!";
	boost::system::error_code error;
	boost::asio::write(socket, boost::asio::buffer(data), error);
	

	if (!error) {
		std::cout << "Client received hello message!\n" << std::endl;
	}
	else {
		std::cout << "send failed: " << error.message() << std::endl;
	}
	system("pause");

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
		//-------receive-------------------------------------------------------------------
		ip::tcp::acceptor receive_acceptor(io_service_, endpoint_);
		std::cout << "acceptor created\n";
		ip::tcp::socket recieve_socket(io_service_/*, protocol_*/);
		std::cout << "socket created\n";

	
		receive_acceptor.accept(recieve_socket);
		std::cout << "Socket ready" << std::endl;
		std::cout << "reading\n";
		std::string message = read_(recieve_socket);
		std::cout << message << std::endl;

		//---------send-----------------------------------------------------------------
		
		//io_service io_service_2;

		ip::tcp::acceptor send_acceptor(io_service_, endpoint_);
		std::cout << " send_acceptor created\n";
		ip::tcp::socket send_socket(io_service_/*, protocol_*/);
		std::cout << " send_socket created\n";
		//send_acceptor.accept(send_socket);
		//send_(send_socket, "Hello From Server!");
		send_(recieve_socket, "Hello From Server!");
		std::cout << "Server sent Hello message to Client!" << std::endl;

	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
	}

	
	//--------------------------------------------------------------------------

	

	//system("pause");

	return EXIT_SUCCESS;
}

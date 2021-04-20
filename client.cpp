#include <iostream>
#include <string>
#include <boost/asio.hpp>
void write_data(boost::asio::ip::tcp::socket& socket)
{
	std::string data = "Hello from client!";
	boost::system::error_code error;
	boost::asio::write(socket, boost::asio::buffer(data), error);

	if (!error) {
		std::cout << "Client sent hello message!\n" << std::endl;
	}
	else {
		std::cout << "send failed: " << error.message() << std::endl;
	}

}

std::string receive_data(boost::asio::ip::tcp::socket& socket) {

	std::cout << "in receive_data\n";

	const std::size_t length = 10;
	char buffer[length];

	boost::asio::streambuf receive_buffer;
	boost::system::error_code error;

	//boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
	boost::asio::read(socket, boost::asio::buffer(buffer, length), error);

	if (error && error != boost::asio::error::eof) {
		std::cout << "receive failed: " << error.message() << std::endl;
	}
	else {
		//const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		//std::cout << "recieved data:"<< data << std::endl;
		return std::string(buffer, length);
	}

	//socket.close();

}



int main(int argc, char** argv)

{
//--------------------------------------------------------------------------
	system("chcp 1251");
	std::string raw_ip_address = "127.0.0.1";
	auto port = 3333;
	boost::system::error_code error_code;
	boost::asio::ip::address ip_address =
		boost::asio::ip::address::from_string(raw_ip_address, error_code);

	if (error_code.value() != 0)
	{
		std::cout << "Failed to parse the IP address. Error code = " <<
			error_code.value() << ". Message: " << error_code.message();

		return error_code.value();
	}
//--------------------------------------------------------------------------

	try
	{
		boost::asio::ip::tcp::endpoint endpoint(ip_address, port);
		std::cout << "Endpoint ready" << std::endl;

		boost::asio::io_service io_service_;
		boost::asio::ip::tcp::socket send_socket(io_service_, endpoint.protocol());
		std::cout << "send_socket created\n";
		send_socket.connect(endpoint);
		std::cout << "send_socket connected" << std::endl;
//--------------------------------------------------------------------------
		write_data(send_socket);
//--------------------------------------------------------------------------
		//boost::asio::io_service io_service_2;

		boost::asio::ip::tcp::socket recieve_socket(io_service_, endpoint.protocol());
		std::cout << "receive_socket created\n";
		recieve_socket.connect(endpoint);
		std::cout << "receive_socket connected" << std::endl;
		//receive_data(recieve_socket);
		std::cout << receive_data(send_socket);
		std::cout << "tjis was received data\n";

	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		//system("pause");
		return e.code().value();
	}



	//system("pause");

	return EXIT_SUCCESS;
}

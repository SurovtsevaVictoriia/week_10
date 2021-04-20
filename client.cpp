#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

void print(std::string text) {
	std::cout << "-------------------------------\n"<<
				"message from server:\n" << 
				 text << 
				"\n-------------------------------\n";
}

void long_write_data(boost::asio::ip::tcp::socket& socket){	
	std::cout << "write_thread started\n";
	std::string text;
	boost::system::error_code error;
	while (std::getline(std::cin, text) && (text != "cancel")) {
		boost::asio::write(socket, boost::asio::buffer(text), error);

		if (!error) {
			std::cout << "Client sent message:\t" << text << std::endl;
		}
		else {
			std::cout << "send failed: " << error.message() << std::endl;
			return;
		}
	}
}

void long_receive_data(boost::asio::ip::tcp::socket& socket) {

	std::cout << "read_thread started\n";

	boost::asio::streambuf buffer;
	boost::system::error_code error;
	boost::asio::read_until(socket, buffer, '\n', error);

	if (!error) {
		std::string message;
		std::istream input_stream(&buffer);
		std::getline(input_stream, message, '\n');
		print(message);
	}
	else {
		std::cout << "send failed: " << error.message() << std::endl;
		return;
	}
	
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
		send_socket.connect(endpoint);
		boost::asio::ip::tcp::socket recieve_socket(io_service_, endpoint.protocol());
		recieve_socket.connect(endpoint);

		std::cout << "soccets connected\n";
//--------------------------------------------------------------------------
		
		std::thread read_tread(long_receive_data, std::ref(recieve_socket));
		std::thread write_thread(long_write_data, std::ref(send_socket));

		read_tread.join();
		write_thread.join();


	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}



	//system("pause");

	return EXIT_SUCCESS;
}

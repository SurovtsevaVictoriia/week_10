#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

void print(std::string text, std::string name) {
	std::cout << "-------------------------------\n" <<
		"message from " << name << ":\n" <<
		text <<
		"\n-------------------------------\n";
}


void long_write_data(boost::asio::ip::tcp::socket& socket) {

	std::cout << "write_thread started\n";

	std::string client_name;
	std::cout << "enter client name\n";
	std::cin >> client_name;
	
	boost::system::error_code error;

	boost::asio::write(socket, boost::asio::buffer(client_name /*+'\n'*/), error);
	if (!error) {
		std::cout << client_name << "send their name to server\t"  << std::endl;
	}
	else {
		std::cout << "send failed: " << error.message() << std::endl;
		return;
	}

	std::string text;
	
	while (std::getline(std::cin, text) && (text != "cancel")) {
		text += '\n';
		boost::asio::write(socket, boost::asio::buffer(text), error);

		if (!error) {
			std::cout << client_name << " sent message:\t" << text << std::endl;
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
	std::string server_name;

	if (!error) {

		std::istream input_stream(&buffer);
		std::getline(input_stream, server_name, '\n');
		std::cout << "server name is " << server_name << std::endl;
	}
	else {
		std::cout << "send failed: " << error.message() << std::endl;
		return;
	}



	while (true) {
		boost::asio::read_until(socket, buffer, '\n', error);

		if (!error) {
			std::string message;
			std::istream input_stream(&buffer);
			std::getline(input_stream, message, '\n');
			print(message, server_name);
		}
		else {
			std::cout << "send failed: " << error.message() << std::endl;
			return;
		}
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
		boost::asio::ip::tcp::endpoint endpoint_1(ip_address, port);
		boost::asio::ip::tcp::endpoint endpoint_2(ip_address, 9999);


		std::cout << "Endpoint ready" << std::endl;

		boost::asio::io_service io_service_;
		boost::asio::ip::tcp::socket send_socket(io_service_, endpoint_1.protocol());
		send_socket.connect(endpoint_1);
		std::cout << "send socket connected\n";
		boost::asio::ip::tcp::socket recieve_socket(io_service_, endpoint_2.protocol());
		recieve_socket.connect(endpoint_2);

		std::cout << "receive soccet connected\n";
		//--------------------------------------------------------------------------

		std::thread write_thread(long_write_data, std::ref(send_socket));
		std::thread read_tread(long_receive_data, std::ref(recieve_socket));

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

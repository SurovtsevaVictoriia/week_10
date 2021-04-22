#include <iostream>
#include <string>
#include <boost/asio.hpp>

void print(std::string text, std::string name) {
	std::cout << "-------------------------------\n" <<
		"|                              |\n" <<
		"|   message from " << name << "\n" <<
		"|   " << text << "\n" <<
		"|                              |" <<
		"\n-------------------------------\n";

	std::cout << "\n"
		<< "|\n"
		<< "|\n"
		<< "^____^\n"
		<< "( O O )\__________________\n"
		<< "(_____)\                  )---<\n"
		<< "       (                  )\n"
		<< "       ||_ _ _ _ _ _ _  W |\n"
		<< "       ||                ||\n";
}
void long_write_data(boost::asio::ip::tcp::socket& socket) {
	std::cout << "write_thread started\n";


	boost::system::error_code error;
	std::string server_name;
	std::cout << "enter server name\n";
	std::cin >> server_name;


	boost::asio::write(socket, boost::asio::buffer(server_name), error);
	if (!error) {
		std::cout << server_name << "send their name to client" << std::endl;
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
			std::cout << "Server sent message:\t" << text << std::endl;
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
	std::string client_name;

	if (!error) {

		std::istream input_stream(&buffer);
		std::getline(input_stream, client_name, '\n');
		std::cout << "client name is " << client_name << std::endl;
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
			print(message, client_name);
		}
		else {
			std::cout << "receive failed: " << error.message() << std::endl;
			return;
		}
	}
	return;
}

int main(int argc, char** argv)
{
	system("chcp 1251");

	using namespace boost::asio;
	//--------------------------------------------------------------------------
	auto port = 3333;


	std::string raw_ip_address = "127.0.0.1";

	boost::system::error_code error_code;
	ip::address ip_address = ip::address::from_string(raw_ip_address, error_code);
	ip::tcp::endpoint endpoint_1(boost::asio::ip::tcp::v4(), port);
	ip::tcp::endpoint endpoint_2(boost::asio::ip::tcp::v4(), 9999);
	std::cout << "Endpoint ready" << std::endl;

	io_service io_service_;
	ip::tcp protocol_ = boost::asio::ip::tcp::v4();
	//--------------------------------------------------------------------------
	try
	{
		//--------------------------------------------------------------------------
		ip::tcp::acceptor receive_acceptor(io_service_, endpoint_1);
		ip::tcp::socket recieve_socket(io_service_);
		receive_acceptor.accept(recieve_socket);
		std::cout << "receive soccet accepted\n";

		ip::tcp::acceptor send_acceptor(io_service_, endpoint_2);
		ip::tcp::socket send_socket(io_service_);
		std::cout << "waiting...\n";
		send_acceptor.accept(send_socket);

		std::cout << " send soccet accepted\n";
		//--------------------------------------------------------------------------

		std::thread read_tread(long_receive_data, std::ref(recieve_socket)/*, std::ref(io_service_), std::ref(endpoint_)*/);
		std::thread write_thread(long_write_data, std::ref(send_socket)/*, std::ref(io_service_), std::ref(endpoint_)*/);

		read_tread.join();
		write_thread.join();



	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
	}


	//--------------------------------------------------------------------------



	//system("pause");

	return EXIT_SUCCESS;
}

#include <iostream>
#include <string>
#include <boost/asio.hpp>

void print(std::string text) {
	std::cout << "-------------------------------\n" <<
		"message from client:\n" <<
		text <<
		"\n-------------------------------\n";
}

void long_write_data(boost::asio::ip::tcp::socket& socket
						/*boost::asio::io_service & io_service_,
						boost::asio::ip::tcp::endpoint endpoint_*/) {
	std::cout << "write_thread started\n";
	/*boost::asio::ip::tcp::acceptor send_acceptor(io_service_, endpoint_);
	boost::asio::ip::tcp::socket send_socket(io_service_);
	send_acceptor.accept(send_socket);*/



	std::string text;
	boost::system::error_code error;
	while (std::getline(std::cin, text) && (text != "cancel")) {
		std::cout << "server sent text: " << text << '\n';
		boost::asio::write(socket, boost::asio::buffer(text), error);

		if (!error) {
			std::cout << "Server sent message:\t" << text << std::endl;
		}
		else {
			std::cout << "send failed: " << error.message() << std::endl;
		}
	}
}

void long_receive_data(boost::asio::ip::tcp::socket& socket
	/*boost::asio::io_service& io_service_, 
	boost::asio::ip::tcp::endpoint endpoint_*/) {


	std::cout << "read_thread started\n";

	/*boost::asio::ip::tcp::acceptor receive_acceptor(io_service_, endpoint_);
	boost::asio::ip::tcp::socket recieve_socket(io_service_);
	receive_acceptor.accept(recieve_socket);
	std::cout << "receive soccet accepted\n";*/

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
	}
	return;
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
		//--------------------------------------------------------------------------
		ip::tcp::acceptor receive_acceptor(io_service_, endpoint_);
		//receive_acceptor.bind(endpoint_);
		//receive_acceptor.listen();
		ip::tcp::socket recieve_socket(io_service_);
		receive_acceptor.accept(recieve_socket);
		std::cout << "receive soccet accepted\n";

		//ip::tcp::acceptor send_acceptor(io_service_, endpoint_);
		//ip::tcp::socket send_socket(io_service_);
		////send_acceptor.accept(send_socket);

		std::cout << "soccets accepted\n";
		//--------------------------------------------------------------------------

		std::thread read_tread(long_receive_data, std::ref(recieve_socket)/*, std::ref(io_service_), std::ref(endpoint_)*/);
		//std::thread write_thread(long_write_data, std::ref(send_socket)/*, std::ref(io_service_), std::ref(endpoint_)*/);

		read_tread.join();
		//write_thread.join();

		

	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
	}

	
	//--------------------------------------------------------------------------

	

	//system("pause");

	return EXIT_SUCCESS;
}

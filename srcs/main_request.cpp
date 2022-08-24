#include "../inc/webserv.hpp"
#include "../inc/Request.hpp"
#include "../inc/response.hpp"

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		Request request(argv[1]);
		request._print_message();
		request._print_dictionary();

		std::cout << std::endl << "request._metod: " << request._method << std::endl;
		std::cout << "request._url: " << request._url << std::endl;
		std::cout << "request._http_version: " << request._http_version << std::endl;
		std::cout << "request._host: " << request._host << std::endl;
	}
	return (0);
}
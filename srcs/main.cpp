//
// Created by amyroshn on 8/16/22.
//
#include "Webserv_machine.hpp"

void test_configs(int ac, char** av) {
	std::cout << "--------------------------------" << std::endl;
	for (int i = 1; i < ac; i++) {
		std::string path = av[i];
		path.insert(0, "config_files_tests/");
		Webserv_machine ws(path.data());
		std::cout << "test=\"" << YELLOW << av[i] << RESET"\" ";
		if (path.find("_ok") != std::string::npos && ws.getErrorMsg() != "OK")
			std::cout << RED"KO"RESET;
		else if (path.find("_ok") != std::string::npos && ws.getErrorMsg() == "OK")
			std::cout << GREEN"OK"RESET;
		else if (path.find("_ok") == std::string::npos && ws.getErrorMsg() == "OK")
			std::cout << RED"KO"RESET;
		else if (path.find("_ok") == std::string::npos && ws.getErrorMsg() != "OK")
			std::cout << GREEN"OK"RESET;
		else
			std::cout << RED"TEST FAILED"RESET;
		std::cout << std::endl;
	}
	std::cout << std::endl << "--------------------------------" << std::endl;
}

int main(int ac, char** av) {
	if (ac == 1)
		std::cout << "No args detected" << std::endl;
	else if (ac == 2) {
		Webserv_machine ws(av[1]);
		//ws.up();
		std::cout << ws.getErrorMsg() << std::endl;
	} else
		test_configs(ac, av);
	//Webserv_machine ws(av[1]);
}

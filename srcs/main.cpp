//
// Created by amyroshn on 8/16/22.
//
#include "../inc/Webserv_machine.hpp"

void test_configs(int ac, char** av) {
	std::cout << "--------------------------------" << std::endl;
	for (int i = 1; i < ac; i++) {
		std::string path = av[i];
		path.insert(0, "config_files_tests/");
		std::string msg;
		Webserv_machine *test;
		try {
			test = new Webserv_machine(path.data());
			msg = test->getErrorMsg();
		} catch (std::exception& e) {
			msg = e.what();
		}
		delete test;
		std::cout << "test=\"" << YELLOW << av[i] << RESET"\" ";
		if ((path.find("_ok") != std::string::npos && !msg.empty())
			|| (path.find("_ok") == std::string::npos && msg.empty()))
			std::cout << RED << "KO" << RESET;
		else if ((path.find("_ok") != std::string::npos && msg.empty())
			|| (path.find("_ok") == std::string::npos && !msg.empty()))
			std::cout << GREEN << "OK" << RESET;
		else
			std::cout << RED << "TEST FAILED" << RESET;
		std::cout << std::endl;
	}
	std::cout << std::endl << "--------------------------------" << std::endl;
}

Webserv_machine *ws;

void handle_exit(int signal) {
	(void) signal;
	ws->setSignal(true);
}

int main(int ac, char** av) {
	if (ac == 1)
		std::cout << "No args detected" << std::endl;
	else if (ac == 2) {
		signal(SIGINT, handle_exit);
		ws = new Webserv_machine(av[1]);
		ws->up();
		std::cout << ws->getErrorMsg() << std::endl;
		delete ws;
	} else
		test_configs(ac, av);
}

//
// Created by amyroshn on 8/16/22.
//
#include "../inc/Webserv_machine.hpp"
#include "CGI_Handler.hpp"

void test_configs(int ac, char** av) {
	std::cout << "--------------------------------" << std::endl;
	std::vector<Server *> servers;
	for (int i = 1; i < ac; i++) {
		std::string path = av[i];
		path.insert(0, "config_files_tests/");
		ConfigParser config(path);
		std::string msg;
		try {
			servers = config.getParsedServers();
			msg = "";
		} catch (std::exception& e) {
			msg = e.what();
		}
		std::vector<Server *>& s = config.getServers();
		for (std::vector<Server *>::iterator it = s.begin(); it != s.end(); it++)
			delete *it;
		std::cout << "test=\"";
		Logger::print(Logger::TXT_MAGENTA, av[i]);
		std::cout << "\" ";
		if ((path.find("_ok") != std::string::npos && !msg.empty())
			|| (path.find("_ok") == std::string::npos && msg.empty()))
			Logger::println(Logger::TXT_RED, "KO");
		else if ((path.find("_ok") != std::string::npos && msg.empty())
			|| (path.find("_ok") == std::string::npos && !msg.empty()))
			Logger::println(Logger::TXT_GREEN, "OK");
		else
			Logger::print(Logger::TXT_YELLOW, Logger::BG_RED, "TEST ERROR");
	}
	std::cout << std::endl << "--------------------------------" << std::endl;
}

Webserv_machine *ws;

void handle_exit(int signal) {
	(void) signal;
	ws->setSignal(true);
}

int main(int ac, char** av) {
	/*
	std::cout << CGI_Handler::launch_cgi(av[1], "dependencies/py", NULL) << std::endl;
	(void) ac;
	*/
	if (ac == 1)
		Logger::println(Logger::TXT_YELLOW, Logger::BG_RED, "No args detected");
	else if (ac == 2) {
		signal(SIGINT, handle_exit);
		ws = new Webserv_machine(av[1]);
		ws->up();
		delete ws;
	} else
		test_configs(ac, av);
	return 0;
}

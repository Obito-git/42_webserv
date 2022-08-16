//
// Created by amyroshn on 8/16/22.
//

#include "Webserv_machine.hpp"

Webserv_machine::Webserv_machine(/* config */ ) {
	Server *serv = new Server();
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

	serv->setAddress(addr);
	serv->getPorts().insert(8080);

	serv->launch();
	while (true) {
		int new_socket;
		int valread;
		const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		int addr_len = sizeof(serv->getAddress());
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept((*serv->getSockets().begin())->getSocketFd(),
								 (struct sockaddr *)&serv->getAddress(),
										 (socklen_t*)&addr_len))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		printf("%s\n",buffer );
		write(new_socket , hello , strlen(hello));
		printf("------------------Hello message sent-------------------");
		close(new_socket);
	}
}

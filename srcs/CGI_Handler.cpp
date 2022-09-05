//
// Created by Anton on 28/08/2022.
//

#include "CGI_Handler.hpp"
// http://www.wijata.com/cgi/cgispec.html#4.0
// https://www.ibm.com/docs/fr/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script

CGI_Handler::CGI_Handler(const Request *req) : _req(req), _env(NULL), _env_len(), _cgi_path(), _status() {
	if (!is_good_type())
		return;
	set_environment();
	if (!_env)
		return;
	launch_cgi();
}

void CGI_Handler::set_environment() {
	const std::map<std::string, std::string> &h = _req->_header;
	std::map<std::string, std::string>::const_iterator it;
	std::map<std::string, std::string> tmp_env;
	
	size_t delim_pos;
	std::string autoris = (it = h.find("Authorization")) == h.end() ? "" : it->second;;
	std::string remote_user;
	if ((delim_pos = autoris.find(' ')) != std::string::npos && delim_pos != autoris.length() - 1) {
		remote_user = autoris.substr(delim_pos + 1);
		autoris = autoris.substr(0, delim_pos);
	}
	tmp_env["AUTH_TYPE"] = autoris;
	tmp_env["CONTENT_LENGTH"] = (it = h.find("Content-Length")) == h.end() ? "" : it->second;;; //FIXME
	tmp_env["CONTENT_TYPE"] = (it = h.find("Content-Type")) == h.end() ? "" : it->second;;
	tmp_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	tmp_env["PATH_INFO"] = _req->_path_info;
	tmp_env["PATH_TRANSLATED"] = _req->_location->getRoot() + _req->_path_info;
	tmp_env["QUERY_STRING"] = _req->_query;
	tmp_env["REMOTE_ADDR"] = _req->_client_socket->getClientIp();
	tmp_env["REMOTE_IDENT"] = remote_user;
	tmp_env["REMOTE_USER"] = remote_user;
	tmp_env["REQUEST_METHOD"] = get_method_name(_req->_method);
	tmp_env["SCRIPT_NAME"] = _req->_url;
	delim_pos = _req->_host.find(':');
	tmp_env["SERVER_NAME"] = delim_pos == std::string::npos ? _req->_host : _req->_host.substr(0, delim_pos);
	tmp_env["SERVER_PORT"] = _req->_client_socket->getClientPort();
	tmp_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	tmp_env["SERVER_SOFTWARE"] = tmp_env.find("SERVER_NAME")->second.append("/HTTP/1.1");
	
	for (it = _req->_header.cbegin(); it != _req->_header.cend(); it++)
		if (!it->first.empty())
			tmp_env.insert(std::make_pair("HTTP_" + it->first, it->second));

	_status = 500;
	char **res = static_cast<char **>(calloc(sizeof(char *), (tmp_env.size() + 1)));
	if (!res)
		return;
	for (it = tmp_env.cbegin(); it != tmp_env.cend(); it++, _env_len++) {
		res[_env_len] = strdup((it->first + "=" + it->second).data());
		if (!res[_env_len]) {
			while (_env_len >= 0)
				free(res[_env_len--]);
			free(res);
			return;
		}
	}
	_env = res;
}

bool CGI_Handler::is_good_type() {
	std::string tmp = _req->_url.substr(_req->_url.find_last_of('.'));
	std::map<std::string, std::string>::const_iterator it = _req->_server->getCgiPaths().find(tmp);
	if (it != _req->_server->getCgiPaths().end()) {
		_status = 501;
		return false;
	}
	_cgi_type = it->first;
	_cgi_path = it->second;
	return true;
}

void CGI_Handler::launch_cgi() {
	int		tube[2];
	pid_t	pid;
	char	buf[BUF_SIZE];
	//char 	args[2][_req->_path_to_requested_file.size()];
	//args[0] = _req->_path_to_requested_file.data();
	char *args[2];
	args[0] = strdup(_req->_path_to_requested_file.data());
	args[1] = NULL;
	memset(buf, 0, BUF_SIZE);

	if (pipe(tube) < 0)
		return ;
	if ((pid = fork()) < 0)
		return ;
	if (pid == 0) {
		int dup_res;
		dup_res = dup2 (tube[1], STDOUT_FILENO);
		close(tube[0]);
		close(tube[1]);
		if (dup_res < 0)
			exit(2);
		if (execve(_cgi_path.data(), args, _env) < 0)
			exit(2);
	} else {
		close(tube[1]);
		while (0 != read(tube[0], buf, sizeof(buf))) {
			_result = _result + buf;
			memset(buf, 0, BUF_SIZE);
		}
		//FIXME close tube[0] ?
		waitpid(pid, NULL, 0);
	}
	if (!_result.empty())
		_status = 200;
	free(args[0]);
}

CGI_Handler::~CGI_Handler() {
	if (!_env)
		return;
	for (int i = 0; i < _env_len; i++)
		free(_env[i]);
	free(_env);
}

int CGI_Handler::getStatus() const {
	return _status;
}

const std::string &CGI_Handler::getResult() const {
	return _result;
}

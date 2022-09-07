//
// Created by Anton on 28/08/2022.
//

#include "CGI_Handler.hpp"
// http://www.wijata.com/cgi/cgispec.html#4.0
// https://www.ibm.com/docs/fr/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script

CGI_Handler::CGI_Handler(const Request *req) : _req(req), _env(NULL), _env_len(), _cgi_path(), _status() {
	_status = 500;
	if (!is_good_type())
		return;
	set_environment();
	if (!_env)
		return;
	_result = launch_cgi(_req->_path_to_requested_file, _cgi_path, _env);
	update_status_and_headers();
}

void CGI_Handler::set_environment() {
	const std::map<std::string, std::string> &h = _req->_header;
	std::map<std::string, std::string>::const_iterator it;
	std::map<std::string, std::string> tmp_env;

	if (_cgi_type == "php")
		tmp_env["REDIRECT_STATUS"] = "200";
	//size_t delim_pos;
	///std::string autoris = (it = h.find("Authorization")) == h.end() ? "" : it->second;;
	//std::string remote_user;
	//if ((delim_pos = autoris.find(' ')) != std::string::npos && delim_pos != autoris.length() - 1) {
	//	remote_user = autoris.substr(delim_pos + 1);
	//	autoris = autoris.substr(0, delim_pos);
	//}
	//tmp_env["AUTH_TYPE"] = autoris;
	tmp_env["CONTENT_LENGTH"] = (it = h.find("Content-Length")) == h.end() ? "" : it->second;;; //FIXME
	tmp_env["CONTENT_TYPE"] = (it = h.find("Content-Type")) == h.end() ? "" : it->second;;
	tmp_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	tmp_env["PATH_INFO"] = _req->_path_info;
	///tmp_env["PATH_TRANSLATED"] = _req->_location->getRoot() + _req->_path_info;
	tmp_env["QUERY_STRING"] = _req->_query.find('?') != std::string::npos ? _req->_query.substr(1) : _req->_query;
	tmp_env["REMOTE_ADDR"] = _req->_client_socket->getClientAddr();
	tmp_env["REQUEST_METHOD"] = get_method_name(_req->_method);
	tmp_env["REQUEST_URI"] = _req->_url;
	tmp_env["SCRIPT_NAME"] = _req->_url;
	tmp_env["SCRIPT_FILENAME"] = _req->_path_to_requested_file;
	tmp_env["DOCUMENT_URI"] = _req->_url;
	tmp_env["DOCUMENT_ROOT"] = _req->_path_to_requested_file.substr(0, _req->_path_to_requested_file.find(_req->_url));
	tmp_env["REMOTE_PORT"] = _req->_client_socket->getClientPort();
	tmp_env["REMOTE_ADDR"] = _req->_client_socket->getClientAddr();
	tmp_env["SERVER_NAME"] = _req->_host.find(':') == std::string::npos ?
			_req->_host : _req->_host.substr(0, _req->_host.find(':'));
	tmp_env["SERVER_PORT"] = _req->_client_socket->getPort();
	tmp_env["SERVER_ADDR"] = _req->_client_socket->getAddr();
	tmp_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	tmp_env["SERVER_SOFTWARE"] = "webserv_amyroshn_okushnir/0.0.1";
	
	
	for (it = _req->_header.cbegin(); it != _req->_header.cend(); it++)
		if (!it->first.empty() && !it->second.empty())
			tmp_env.insert(std::make_pair("HTTP_" + ft_to_upper_case(it->first), it->second));
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
	_cgi_type = _req->_extention;
	std::map<std::string, std::string>::const_iterator it = _req->_server->getCgiPaths().find(_cgi_type);
	if (it == _req->_server->getCgiPaths().end()) {
		_status = 501;
		return false;
	}
	_cgi_path = it->second;
	return true;
}

std::string CGI_Handler::launch_cgi(const std::string &file_path, const std::string &cgi_path, char **env) {
	int				tube[2];
	pid_t			pid;
	char			buf[BUF_SIZE];
	std::string		res;
	char			*args[3];

	args[0] = const_cast<char *>(cgi_path.data());
	args[1] = const_cast<char *>(file_path.data());
	args[2] = NULL;
	memset(buf, 0, BUF_SIZE);
	if (pipe(tube) < 0)
		return "Status: 500 Internal Server Error";
	if ((pid = fork()) < 0)
		return "Status: 500 Internal Server Error";
	if (pid == 0) {
		int dup_res;
		dup_res = dup2(tube[1], STDOUT_FILENO);
		close(tube[0]);
		close(tube[1]);
		if (dup_res < 0)
			exit(2);
		if (execve(cgi_path.data(), args, env) < 0)
			exit(2);
	} else {
		close(tube[1]);
		while (read(tube[0], buf, BUF_SIZE - 1) > 0) {
			res.append(buf);
			memset(buf, 0, BUF_SIZE);
		}
		waitpid(pid, NULL, 0);
	}
	if (res.empty())
		return "Status: 500 Internal Server Error";
	return res;
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

void CGI_Handler::update_status_and_headers() {
	if (_result.find("Status: ") == 0) {
		_status = atoi(_result.data() + 8);
		return ;
	}
	_status = 200;
	if (_cgi_type == "py") {
		std::string tmp = "X-Powered-By: python3\r\n";
		tmp.append("Content-type: text/html; charset=UTF-8\r\n\r\n");
		_result = tmp.append(_result);
	}
	std::stringstream ss;
	ss << "Content-Length: ";
	size_t position = _result.find("\r\n\r\n");
	if (position == std::string::npos)
		position = _result.find("\n\n");
	if (position == std::string::npos) {
		_status = 500;
		return;
	}
	while (isspace(_result[position]))
		position++;
	ss << _result.substr(position).size() << "\r\n";
	_result = ss.str().append(_result);
}

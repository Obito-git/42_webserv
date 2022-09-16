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
	_result = launch_cgi(_req->getPathToRequestedFile(), _cgi_path, _req->getRequestBody(), _env);
	update_status_and_headers();
}

void CGI_Handler::set_environment() {
	const std::map<std::string, std::string> &h = _req->getHeader();
	std::map<std::string, std::string>::const_iterator it;
	std::map<std::string, std::string> tmp_env;

	if (_cgi_type == "php")
		tmp_env["REDIRECT_STATUS"] = "200";
	tmp_env["CONTENT_LENGTH"] = (it = h.find("Content-Length")) == h.end() ? "" : it->second;
	tmp_env["CONTENT_TYPE"] = (it = h.find("Content-Type")) == h.end() ? "" : it->second;;
	tmp_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	tmp_env["PATH_INFO"] = _req->getPathInfo();
	tmp_env["QUERY_STRING"] = _req->getQuery().find('?') != std::string::npos ? _req->getQuery().substr(1) : _req->getQuery();
	tmp_env["REMOTE_ADDR"] = _req->getClientSocket()->getClientAddr();
	tmp_env["REQUEST_METHOD"] = get_method_name(_req->getMethod());
	tmp_env["REQUEST_URI"] = _req->getUrl();
	tmp_env["SCRIPT_NAME"] = _req->getUrl();
	tmp_env["SCRIPT_FILENAME"] = _req->getPathToRequestedFile();
	tmp_env["DOCUMENT_URI"] = _req->getUrl();
	tmp_env["DOCUMENT_ROOT"] = _req->getPathToRequestedFile().
			substr(0, _req->getPathToRequestedFile().find(_req->getUrl()));
	tmp_env["REMOTE_PORT"] = _req->getClientSocket()->getClientPort();
	tmp_env["REMOTE_ADDR"] = _req->getClientSocket()->getClientAddr();
	tmp_env["SERVER_NAME"] = _req->getHost().find(':') == std::string::npos ?
			_req->getHost() : _req->getHost().substr(0, _req->getHost().find(':'));
	tmp_env["SERVER_PORT"] = _req->getClientSocket()->getPort();
	tmp_env["SERVER_ADDR"] = _req->getClientSocket()->getAddr();
	tmp_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	tmp_env["SERVER_SOFTWARE"] = "webserv_amyroshn_okushnir/0.0.1";
	
	
	for (it = _req->getHeader().begin(); it != _req->getHeader().end(); it++)
		if (!it->first.empty() && !it->second.empty())
			tmp_env.insert(std::make_pair("HTTP_" + ft_to_upper_case(it->first), it->second));
	char **res = static_cast<char **>(calloc(sizeof(char *), (tmp_env.size() + 1)));
	if (!res)
		return;
	for (it = tmp_env.begin(); it != tmp_env.end(); it++, _env_len++) {
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
	_cgi_type = _req->getExtention();
	std::map<std::string, std::string>::const_iterator it = _req->getServer()->getCgiPaths().find(_cgi_type);
	if (it == _req->getServer()->getCgiPaths().end()) {
		_status = 501;
		return false;
	}
	_cgi_path = it->second;
	return true;
}

std::string CGI_Handler::launch_cgi(const std::string &file_path, const std::string &cgi_path, const std::string &body, char **env) {
	int				tube[2];
	pid_t			pid;
	char			buf[BUF_SIZE];
	std::string		res;
	char			*args[3];
	FILE			*body_file;
	int				bodyfile_fd;
	
	if (!(body_file = tmpfile()))
		return "Status: 500 Internal Server Error";
	if ((bodyfile_fd = fileno(body_file)) == -1) {
		fclose(body_file);
		return "Status: 500 Internal Server Error";
	}
	args[0] = const_cast<char *>(cgi_path.data());
	args[1] = const_cast<char *>(file_path.data());
	args[2] = NULL;
	memset(buf, 0, BUF_SIZE);
	if (pipe(tube) < 0)
		return "Status: 500 Internal Server Error";
	if ((pid = fork()) < 0)
		return "Status: 500 Internal Server Error";
	if (pid == 0) {
		if (write(bodyfile_fd, body.data(), body.length()) == -1)
			exit(2);
		lseek(bodyfile_fd, 0, SEEK_SET);
		int dup_res;
		dup_res = dup2(bodyfile_fd, STDIN_FILENO);
		if (dup_res != -1)
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
		close(tube[0]);
	}
	fclose(body_file);
	close(bodyfile_fd);
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
	if (_req->getHeader().find("Connection") != _req->getHeader().end())
		_result = std::string("Connection: ") + _req->getHeader().find("Connection")->second + "\r\n" + _result;
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

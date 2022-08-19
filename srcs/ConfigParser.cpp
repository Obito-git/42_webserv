//
// Created by Anton on 19/08/2022.
//

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &path) : _path(path) {}

//getter
const std::vector<Server *> &ConfigParser::getServers() {
	if (_servers.empty())
		parse_config(_path.data());
	return _servers;
}

/******************************************************************************************************************
 ************************************************** PARSING *******************************************************
 *****************************************************************************************************************/

void ConfigParser::parse_config(const char *path) {
	std::string file_content;
	//trying to open file and checks minimal requirements
	try {
		file_content = ft_read_file(path);
	} catch (std::exception& e) {
		throw ConfigCriticalError("Cannot open config file\n");
	}
	if (file_content.length() < 28 || file_content.find("server") == std::string::npos ||
		file_content.find("listen") == std::string::npos || file_content.find("port") == std::string::npos)
		throw ConfigNoRequiredKeywords();
	//will read all file_content and delete already wrote part from file_content
	str_iter it;
	int current_line_number = 1;
	for (it = file_content.begin(); file_content.length() && it != file_content.end(); it++) {
		//trying to find server block
		it = skip_comments_and_spaces(file_content, it, current_line_number);
		if (file_content.length() < 2)
			throw ConfigNoRequiredKeywords();
		if (file_content.find_first_of("server") != 0)
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content, "server", current_line_number).data());
		it = file_content.erase(it, it + 6);
		it = skip_comments_and_spaces(file_content, it, current_line_number);
		// if server keyword is found, trying to find { and call parsing block function
		if (*it != '{')
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content,"{", current_line_number).data());
		parse_server_block(file_content, ++it, current_line_number);
		it = skip_comments_and_spaces(file_content, it, current_line_number);
	}
}

ConfigParser::str_iter ConfigParser::skip_comments_and_spaces(std::string& file_content, str_iter it, int& line) {
	while ((isspace(*it) && it != file_content.end()) || *it == '#') {
		if (*it == '#')
			while (it != file_content.end() && *it != '\n')
				it++;
		if (*it == '\n')
			line++;
		it++;
	}
	//return file_content.begin() == it ? it : file_content.erase(file_content.begin(), it);
	if (file_content.begin() == it)
		return it;
	else if (it == file_content.end()) {
		file_content.clear();
		return file_content.end();
	}
	return file_content.erase(file_content.begin(), it);
}

std::string& ConfigParser::find_unexpected_token(std::string &s, const char* token, int line) {
	std::string tmp = s;
	_error_msg = "Error, unexpected token. ";
	_error_msg.append(token).append(" expected at line ");
	_error_msg.append(std::to_string(line)).append(":\n");
	_error_msg.append(tmp.find('\n') == std::string::npos ? tmp :
					  tmp.substr(0, tmp.find('\n')));
	return _error_msg;
}

/******************************************************************************************************************
 **************************************** SERVER BLOCK PARSING ****************************************************
 *****************************************************************************************************************/

void ConfigParser::parse_server_block(std::string &file_content, str_iter &it, int &line) {
	//creates new server and saves it
	Server* s = new Server;
	_servers.push_back(s);
	it = skip_comments_and_spaces(file_content, it, line);
	if (*it == '}')
		throw ConfigUnexpectedToken(find_unexpected_token(
				file_content, "Empty server block is detected, at least listen and port", line).data());
	while (*it != '}' && it != file_content.end()) {
		//trying to find a keyword defined in _server_keywords str array
		int keyword_index = find_server_keyword(file_content, it, line);
		//parsing of arguments of keyword
		std::vector<std::string> args;
		while (it != file_content.end()) {
			if (isspace(*it)) {
				args.push_back(file_content.substr(0, it - file_content.begin()));
				while (isspace(*it) && *it != '\n')
					it++;
				it = file_content.erase(file_content.begin(), it);
				if (*it == '\n')
					break;
			}
			it++;
		}
		it = file_content.erase(file_content.begin(), it);
		if (it == file_content.end())
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content,"}", line).data());
		//filling of server settings
		switch (keyword_index) {
			case ::LISTEN:
				parse_listen_args(s, args, line, file_content);
				break;
			case ::PORT:
				parse_port_args(s, args, line, file_content);
				break;
			case ::SERVER_NAME:
				parse_servername_args(s, args, line, file_content);
				break;
			case ::ERR_PAGE:
				parse_errorpages_args(s->getSettings(), args, line, file_content);
				break;
			case ::CLIENT_BODY_SIZE:
				parse_bodysize_args(s->getSettings(), args, line, file_content);
				break;
			case ::FILE_UPLOAD:
				parse_fileupload_args(s->getSettings(), args, line, file_content);
				break;
			case ::METHODS:
				parse_methods_args(s->getSettings(), args, line, file_content);
				break;
			case ::INDEX:
				parse_index_args(s->getSettings(), args, line, file_content);
				break;
			case ::AUTOINDEX:
				parse_autoindex_args(s->getSettings(), args, line, file_content);
				break;
			default:
				;
		}
		it = skip_comments_and_spaces(file_content, it, line);
		if (it == file_content.end())
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content, "}", line).data());
	}
	it++;
}



/******************************************************************************************************************
 ************************************* PARAM ARGUMENTS PARSING ****************************************************
 *****************************************************************************************************************/


void
ConfigParser::parse_listen_args(Server *s, std::vector<std::string> &args, int line, std::string &file_content) {
	if (args.size() != 1) {
		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
			_error_msg.append(*it).append(" ");
		throw ConfigUnexpectedToken(find_unexpected_token(
				_error_msg, "only one ip address/host", line).data());
	}
	s->setIp(*args.begin());
}

//trying to find a keyword and check's if they are arguments after it
int ConfigParser::find_server_keyword(std::string &file_content, str_iter &it, int& line) {
	unsigned long pos = file_content.find_first_of(" \t\r\v\f\n");
	std::string tmp = file_content.substr(0, pos);
	it += static_cast<std::string::difference_type>(pos);
	while (*it && *it != '\n' && isspace(*it))
		it++;
	if (pos == std::string::npos || *it == '\n')
		throw ConfigUnexpectedToken(find_unexpected_token(file_content,
														  "argument after keyword", line).data());
	int y;
	for (y = 0; y < MAX_SERV_KEYWORDS; y++)
		if (tmp == Server::_server_keywords[y])
			break;
	if (y == MAX_SERV_KEYWORDS) {
		tmp = "Expected one of [";
		for (int x = 0; x < MAX_SERV_KEYWORDS; x++)
			if (x != MAX_SERV_KEYWORDS - 1)
				tmp.append(Server::_server_keywords[x]).append(", ");
		tmp.append("] in server block");
		throw ConfigUnexpectedToken(find_unexpected_token(
				file_content,tmp.data(), line).data());
	}
	it = skip_comments_and_spaces(file_content, it, line);
	return y;
}

void ConfigParser::parse_port_args(Server *s, std::vector<std::string> &args, int line, std::string& file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		str_iter y;
		for (y = (*it).begin(); y != (*it).end() && (isdigit(*y)); y++);
		int port = atoi((*it).data());
		if (y != (*it).end() || ((port < 1024 || port > 65535) && port != 80))
			throw ConfigUnexpectedToken(find_unexpected_token(
					*it, "decimal value of port from 1024 to 65535 or 80", line).data());
		s->setPorts(port);
	}
}

void
ConfigParser::parse_servername_args(Server *s, std::vector<std::string> &args, int line, std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		for (std::vector<Server *>::iterator it_serv = _servers.begin(); it_serv != _servers.end(); it_serv++) {
			for (std::vector<std::string>::const_iterator it_sname = (*it_serv)->getServerName().cbegin();
				 it_sname != (*it_serv)->getServerName().cend(); it_sname++) {
				if (*it == *it_sname)
					throw ConfigUnexpectedToken(find_unexpected_token(
							file_content,"This server_name is already exist."
										 " Uniq server_name", line).data());
				s->setServerName(*it);
			}
		}
	}
}

void
ConfigParser::parse_errorpages_args(Location& loc, std::vector<std::string> &args, int line, std::string &file_content) {
	int err_code;
	std::string err_page;

	if (args.size() == 2) {
		str_iter y;
		std::string err_page_code_str = *args.begin();
		for (y = err_page_code_str.begin(); y != err_page_code_str.end() && (isdigit(*y)); y++);
		err_code = atoi(err_page_code_str.data());
		if (y != err_page_code_str.end() ||
			((err_code < 400 || err_code > 417) && (err_code < 500 || err_code > 505)))
			throw ConfigUnexpectedToken(find_unexpected_token(
					err_page_code_str, "error code from 400 to 417 or from 500 to 505", line).data());
		try {
			err_page = ft_read_file(*(args.begin() + 1));
		} catch (std::exception& e) {
			_error_msg = "Cannot open error page ";
			_error_msg.append(*(args.begin() + 1));
			throw ConfigCriticalError(_error_msg.data());
		}
	} else {
		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
			_error_msg.append(*it).append(" ");
		throw ConfigUnexpectedToken(find_unexpected_token(
				_error_msg, "error_page [STATUS CODE] [PATH] pattern", line).data());
	}
	loc.setErrorPages(static_cast<short>(err_code), std::make_pair(*(args.begin() + 1), err_page));
}

void
ConfigParser::parse_bodysize_args(Location& loc, std::vector<std::string> &args, int line, std::string &file_content) {
	long body_size;
	if (args.size() == 1) {
		str_iter y;
		std::string body_size_str = *args.begin();
		for (y = body_size_str.begin(); y != body_size_str.end() && (isdigit(*y)); y++);
		body_size = atol(body_size_str.data());
		if (y != body_size_str.end() || body_size < 0)
			throw ConfigUnexpectedToken(find_unexpected_token(
					body_size_str, "positive number", line).data());
	} else {
		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
			_error_msg.append(*it).append(" ");
		throw ConfigUnexpectedToken(find_unexpected_token(
				_error_msg, "positive number", line).data());
	}
	loc.setMaxBodySize(static_cast<unsigned long>(body_size));
}

void
ConfigParser::parse_fileupload_args(Location& loc, std::vector<std::string> &args, int line, std::string &file_content) {
	std::string str = *args.begin();
	if (args.size() != 1 || (str != "on" && str != "off"))
		throw ConfigUnexpectedToken(find_unexpected_token(
				str, "on or off", line).data());
	loc.setFileUpload(str == "on");
}

void ConfigParser::parse_methods_args(Location &loc, std::vector<std::string> &args, int line,
										 std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		if (*it == "GET")
			loc.setAllowedMethods(GET);
		else if (*it == "POST")
			loc.setAllowedMethods(POST);
		else if (*it == "DELETE")
			loc.setAllowedMethods(DELETE);
		else {
			for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
				_error_msg.append(*it).append(" ");
			throw ConfigUnexpectedToken(find_unexpected_token(
					_error_msg, "GET/POST/DELETE methods", line).data());
		}
	}
}

void
ConfigParser::parse_index_args(Location &loc, std::vector<std::string> &args, int line, std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		loc.setIndex(*it);
	}
}

void ConfigParser::parse_autoindex_args(Location &loc, std::vector<std::string> &args, int line,
										   std::string &file_content) {
	std::string str = *args.begin();
	if (args.size() != 1 || (str != "on" && str != "off"))
		throw ConfigUnexpectedToken(find_unexpected_token(
				str, "on or off", line).data());
	loc.setAutoindex(str == "on");
}

/******************************************************************************************************************
 *********************************************** EXCEPTIONS *******************************************************
 *****************************************************************************************************************/

ConfigParser::ConfigUnexpectedToken::ConfigUnexpectedToken(const char *m): msg(m) {}

const char *ConfigParser::ConfigUnexpectedToken::what() const throw() {
	return msg;
}

ConfigParser::ConfigCriticalError::ConfigCriticalError(const char *m): msg(m) {}

const char *ConfigParser::ConfigCriticalError::what() const throw() {
	return msg;
}

const char *ConfigParser::ConfigNoRequiredKeywords::what() const throw() {
	return ("Your config must have as least one server block "
			"with listen and port parameters like:\n"
			"server {\n\tlisten [IP ADDRESS]\n\tport [PORT]\n}");
}

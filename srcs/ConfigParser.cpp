//
// Created by Anton on 19/08/2022.
//

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &path) : _path(path), _line_number(1) {}

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
	str_iter it;
	
	//trying to open file and checks minimal requirements
	try {
		file_content = ft_read_file(path).append(" ");
	} catch (std::exception& e) {
		throw ConfigCriticalError("Cannot open config file\n");
	}
	//will read all file_content and delete already wrote part from file_content
	for (it = file_content.begin(); file_content.length() && it != file_content.end(); it++) {
		//trying to find server block
		it = skip_comments_and_spaces(file_content, it);
		if (file_content.find("server") != 0)
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content, "server").data());
		it = file_content.erase(it, it + 6);
		it = skip_comments_and_spaces(file_content, it);
		// if server keyword is found, trying to find { and call parsing block function
		if (*it != '{')
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content,"{").data());
		parse_server_block(file_content, ++it);
		it = skip_comments_and_spaces(file_content, it);
	}
}

ConfigParser::str_iter ConfigParser::skip_comments_and_spaces(std::string& file_content, str_iter it) {
	while ((isspace(*it) && it != file_content.end()) || *it == '#') {
		if (*it == '#')
			while (it != file_content.end() && *it != '\n')
				it++;
		if (*it == '\n')
			_line_number++;
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

std::string& ConfigParser::find_unexpected_token(std::string s, const char* token) {
	_error_msg = "Error, unexpected token. ";
	_error_msg.append(token).append(" expected at line ");
	_error_msg.append(std::to_string(_line_number)).append(":\n");
	_error_msg.append(s.find('\n') == std::string::npos ? s :
					  s.substr(0, s.find('\n')));
	return _error_msg;
}

/******************************************************************************************************************
 **************************************** SERVER BLOCK PARSING ****************************************************
 *****************************************************************************************************************/

std::vector<std::string> ConfigParser::parse_parameter_args(std::string& file_content, str_iter &it) {
	std::vector<std::string> args;
	while (it != file_content.end() && *it != '#') {
		if (isspace(*it)) {
			args.push_back(file_content.substr(0, it - file_content.begin()));
			while (isspace(*it) && *it != '\n')
				it++;
			it = file_content.erase(file_content.begin(), it);
			if (*it == '\n')
				break;
			continue ;
		}
		it++;
	}
	it = file_content.erase(file_content.begin(), it);
	return (args);
}

void ConfigParser::parse_server_block(std::string &file_content, str_iter &it) {
	//creates new server and saves it
	Server* s = new Server;
	_servers.push_back(s);
	it = skip_comments_and_spaces(file_content, it);
	while (*it != '}' && it != file_content.end()) {
		//trying to find a keyword defined in Server::_server_keywords str array
		int keyword_index = find_server_keyword(file_content, it);
		//parsing of arguments of keyword
		std::vector<std::string> args(parse_parameter_args(file_content, it));
		//filling of server settings
		switch (keyword_index) {
			case ::LISTEN: parse_listen_args(s, args,  file_content); break;
			case ::PORT: parse_port_args(s, args,  file_content); break;
			case ::SERVER_NAME: parse_servername_args(s, args,  file_content); break;
			case ::ERR_PAGE: parse_errorpages_args(s->getSettings(), args,  file_content); break;
			case ::CLIENT_BODY_SIZE: parse_bodysize_args(s->getSettings(), args,  file_content); break;
			case ::FILE_UPLOAD: parse_fileupload_args(s->getSettings(), args,  file_content); break;
			case ::METHODS: parse_methods_args(s->getSettings(), args,  file_content); break;
			case ::INDEX: parse_index_args(s->getSettings(), args,  file_content); break;
			case ::AUTOINDEX: parse_autoindex_args(s->getSettings(), args,  file_content); break;
			default:
				;
		}
		it = skip_comments_and_spaces(file_content, it);
		if (it == file_content.end())
			throw ConfigUnexpectedToken(find_unexpected_token(
					file_content, "}").data());
	}
	it++;
	if (s->getIp().empty() || s->getPorts().empty())
		throw ConfigNoRequiredKeywords();
}



/******************************************************************************************************************
 ************************************* PARAM ARGUMENTS PARSING ****************************************************
 *****************************************************************************************************************/

//trying to find a keyword and check's if they are arguments after it
int ConfigParser::find_server_keyword(std::string &file_content, str_iter &it) {
	unsigned long pos = file_content.find_first_of(" \t\r\v\f\n");
	std::string tmp = file_content.substr(0, pos);
	it += static_cast<std::string::difference_type>(pos);
	while (*it && *it != '\n' && isspace(*it))
		it++;
	if (pos == std::string::npos || *it == '\n')
		throw ConfigUnexpectedToken(find_unexpected_token(file_content,
														  "argument after keyword").data());
	int y;
	for (y = 0; y < MAX_SERV_KEYWORDS; y++)
		if (tmp == Server::_server_keywords[y])
			break;
	if (y == MAX_SERV_KEYWORDS) {
		tmp = "Expected one of [";
		for (int x = 0; x < MAX_SERV_KEYWORDS; x++) {
			tmp.append(Server::_server_keywords[x]);
			x < MAX_SERV_KEYWORDS - 1 ? tmp.append(", ") : tmp.append("] in server block");
		}
		throw ConfigUnexpectedToken(find_unexpected_token(
				file_content,tmp.data()).data());
	}
	it = skip_comments_and_spaces(file_content, it);
	return y;
}

void ConfigParser::parse_listen_args(Server *s, std::vector<std::string> &args, std::string &file_content) {
	if (args.size() != 1)
		throw ConfigUnexpectedToken(find_unexpected_token(ft_strjoin(args.begin(),
										 args.end(), " "), "only one ip address/host").data());
	s->setIp(*args.begin());
}

void ConfigParser::parse_port_args(Server *s, std::vector<std::string> &args, std::string& file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		int port = atoi((*it).data());
		if ((*it).find_first_not_of("0123456789") != std::string::npos ||
		((port < 1024 || port > 65535) && port != 80))
			throw ConfigUnexpectedToken(find_unexpected_token(
					*it, "decimal value of port from 1024 to 65535 or 80").data());
		s->setPorts(port);
	}
}

void
ConfigParser::parse_servername_args(Server *s, std::vector<std::string> &args, std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		for (std::vector<Server *>::iterator it_serv = _servers.begin(); it_serv != _servers.end(); it_serv++) {
			for (std::vector<std::string>::const_iterator it_sname = (*it_serv)->getServerName().cbegin();
				 it_sname != (*it_serv)->getServerName().cend(); it_sname++) {
				if (*it == *it_sname && (*it).empty())
					throw ConfigUnexpectedToken(find_unexpected_token(file_content,
									"Can't have 2 servers without server_name").data());
				else if (*it == *it_sname)
					throw ConfigUnexpectedToken(find_unexpected_token(
							file_content,"Server is already exist."
										 " Uniq server_name").data());
				s->setServerName(*it);
			}
		}
	}
}

void
ConfigParser::parse_errorpages_args(Location& loc, std::vector<std::string> &args, std::string &file_content) {
	int err_code;
	std::string err_page;

	if (args.size() == 2) {
		err_code = atoi((*args.begin()).data());
		if (args.begin()->find_first_not_of("0123456789") != std::string::npos ||
			((err_code < 400 || err_code > 417) && (err_code < 500 || err_code > 505)))
			throw ConfigUnexpectedToken(find_unexpected_token(
					*args.begin(), "error code from 400 to 417 or from 500 to 505").data());
		try {
			err_page = ft_read_file(*(args.begin() + 1));
		} catch (std::exception& e) {
			_error_msg = "Cannot open error page ";
			_error_msg.append(*(args.begin() + 1));
			throw ConfigCriticalError(_error_msg.data());
		}
	} else
		throw ConfigUnexpectedToken(find_unexpected_token(ft_strjoin(args.begin(), args.end(),
																	 " "),
														  "error_page [STATUS CODE] [PATH] pattern").data());
	loc.setErrorPages(static_cast<short>(err_code), std::make_pair(*(args.begin() + 1), err_page));
}

void ConfigParser::parse_bodysize_args(Location& loc, std::vector<std::string> &args, std::string &file_content) {
	long body_size = atol((*args.begin()).data());
	if (args.size() != 1 || (*args.begin()).find_first_not_of("0123456789") != std::string::npos || body_size < 0)
		throw ConfigUnexpectedToken(find_unexpected_token(*args.begin(), "positive number").data());
	loc.setMaxBodySize(static_cast<unsigned long>(body_size));
}

void
ConfigParser::parse_fileupload_args(Location& loc, std::vector<std::string> &args, std::string &file_content) {
	std::string str = *args.begin();
	if (args.size() != 1 || (str != "on" && str != "off"))
		throw ConfigUnexpectedToken(find_unexpected_token(
				str, "on or off").data());
	loc.setFileUpload(str == "on");
}

void ConfigParser::parse_methods_args(Location &loc, std::vector<std::string> &args,
										 std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		if (*it == "GET")
			loc.setAllowedMethods(GET);
		else if (*it == "POST")
			loc.setAllowedMethods(POST);
		else if (*it == "DELETE")
			loc.setAllowedMethods(DELETE);
		else
			throw ConfigUnexpectedToken(find_unexpected_token(*it, "GET/POST/DELETE methods").data());
	}
}

void ConfigParser::parse_index_args(Location &loc, std::vector<std::string> &args, std::string &file_content) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		loc.setIndex(*it);
}

void ConfigParser::parse_autoindex_args(Location &loc, std::vector<std::string> &args, std::string &file_content) {
	std::string str = *args.begin();
	if (args.size() != 1 || (str != "on" && str != "off"))
		throw ConfigUnexpectedToken(find_unexpected_token(
				str, "on or off").data());
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

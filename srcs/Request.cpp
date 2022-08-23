#include "Request.hpp"

Request::Request(): _method(INIT), _url(""), _http_version(""), _host(""),
			_header(std::map<std::string, std::string>()),
			_message(std::vector<std::string>()), _responce(responce()) {};


Request::Request(const char *message, Webserv_machine* webserv): _method(INIT), _url(""), _http_version(""), _host(""),
														   _header(std::map<std::string, std::string>()),
														   _message(std::vector<std::string>()), _responce(responce()),
														   ws(webserv)
														   
{
	_read_message(message);
	_make_map_of_headers();
	_fill_up_request();
}

Request::Request(const Request &other) : _method(other._method), _url(other._url),
	_http_version(other._http_version), _host(other._host), _header(other._header), _message(other._message) {};

Request& Request::operator=(const Request &other)
{
	_method = other._method;
	_url = other._url;
	_http_version = other._http_version;
	_host = other._host;
	_header = other._header;
	_message = other._message;
	return(*this);
}


Request::~Request() {};

// RESPONCE

std::string	Request::_make_reponce(std::string version, int code, std::string msg)
{
	std::stringstream buf;

	_responce._http_version = version;
	_responce._status_code = code;
	_responce._status_message = msg;

	std::time_t now = time(0);
	tm *gmtm = gmtime(&now);
	char* date = asctime(gmtm);

	buf << version << " " << code << " " << msg << std::endl;
	buf << "Date: " << date;
	buf << "Server:" << "Webserver" << std::endl;
	buf << "Content-Type:" << "text/html" << std::endl;

	return (buf.str());
}

// CHECKER

int	Request::_check_first_line()
{
	std::string elem;
	size_t pos = (_message[0]).find(" ");
	if (pos == 0 || pos == std::string::npos)
	{
		_rep = _make_reponce("HTTP/1.0", 400, "Bad Request");
		return (1);
	}
	elem = (_message[0]).substr(0,pos);
	if (elem.compare("GET") && elem.compare("POST") && elem.compare("DELETE"))
	{
		_rep = _make_reponce("HTTP/1.1", 405, "Method Not Allowed");
		return (1);
	}
	pos = (_message[0]).find_last_of(" ");
	elem = (_message[0]).substr(pos + 1);
	if (elem.compare("HTTP/1.0") && elem.compare("HTTP/1.1") &&
			elem.compare("HTTP/2") && elem.compare("HTTP/3"))
	{
		_rep = _make_reponce("HTTP/1.0", 400, "Bad Request");
		return (1);
	}
	return (0);
}

int	Request::_check_second_line() // \n ???
{
	if (_message[1][0] == '\t' || _message[1][0] == 0
		|| _message[1][0] == '\v' || _message[1][0] == '\f' || _message[1][0] == '\r' || _message[1][0] == ' ')
	{
		_rep = _make_reponce("HTTP/1.0", 400, "Bad Request");
		return (1);
	}
	return (0);
}


// HELPERS PARSING
void	Request::_read_message(const char * filename)
{
	std::ifstream	text;
	std::string		line;

	text.open(filename, std::ifstream::in);
	std::getline(text, line);
	_message.push_back(line);
	if (!_check_first_line())
	{
		std::getline(text, line);
		_message.push_back(line);
		if (!_check_second_line())
		{
			while (!text.eof())
			{
				std::getline(text, line);
				_message.push_back(line);
			}
		}
		else
			std::cout << _rep;
	}
	else
		std::cout << _rep;
	text.close();
}

// MAKE MAP OF HEADERS

void	Request::_make_map_of_headers()
{
	std::string key;
	std::string value;
	size_t pos = 0;

	std::vector<std::string>::iterator it = _message.begin();
	it++;
	for(; it < _message.end(); ++it)
	{
		pos = (*it).find(":");
		key = (*it).substr(0,pos);
		pos = (*it).find(" "); // attention, pas toujours
		value = (*it).substr(pos + 1);
		_header.insert(std::make_pair(key, value));
	}
}

// HELPERS FOR PRINTING

void Request::_print_message()
{
	std::vector<std::string>::iterator first = _message.begin();
	std::cout << std::endl;
	for (; first != _message.end(); first++)
		std::cout << *first << std::endl;
}

void Request::_print_dictionary()
{
	std::map<std::string, std::string>::iterator first = _header.begin();
	for (; first != _header.end(); first++)
	{
		std::cout << "key : " <<
		(*first).first << " value : " << (*first).second << std::endl;
	}
}

// FILL_UP_REQUEST

void	Request::_fill_up_method(std::string elem)
{
	if (elem.compare("GET") == 0)
		this->_method = GET;
	else if (elem.compare("POST") == 0)
		this->_method = POST;
	else if (elem.compare("DELETE") == 0)
		this->_method = DELETE;
	else
		this->_method = OTHER;
}

void	Request::_fill_up_url(std::string line)
{
	size_t pos = (line).find(" ");
	size_t pos1 = (line).find_last_of(" ");

	std::string elem = (line).substr(pos + 1, pos1 - pos - 1);
	this->_url = elem;
}

void	Request::_fill_up_protocol(std::string line)
{
	size_t pos = (line).find_last_of(" ");
	std::string elem = (line).substr(pos + 1);
	this->_http_version = elem;
}

void	Request::_fill_up_host(std::map<std::string, std::string>::iterator it)
{
	this->_host = (*it).second;
}

int	Request::_fill_up_request()
{
	std::string elem;
	size_t pos = (_message[0]).find(" ");
	elem = (_message[0]).substr(0,pos);
	_fill_up_method(elem);
	_fill_up_url(_message[0]);
	_fill_up_protocol(_message[0]);
	std::map<std::string, std::string>::iterator it;
	it = _header.find("Host");
	if (it != _header.end())
		_fill_up_host(it);
	// else
	// ?on fais quoi? erreur ou on mis le host par default?
	return (0);
}

std::string Request::generate_error_body(Location &location, short status_code) {
	const std::map<short, std::string>& error_pages = location.getErrorPages();
	std::stringstream code;
	code << status_code << " " << HttpStatus::reasonPhrase(status_code);
	std::map<short, std::string>::const_iterator it = error_pages.find(status_code);
	if (it != error_pages.end())
		return (*it).second;
	std::string s = "<html>\n<head><title>";
	s.append(code.str());
	s.append("</title></head>\n<body bgcolor=\"black\">\n<p><img style=\"display: block; margin-left: auto;"
			 "margin-right: auto;\"src=\"https://http.cat/").append(code.str().substr(0,code.str().find(' ')));
	s.append("\" alt=\"");
	s.append(code.str()).append(" width=\"750\" height=\"600\"/></p>\n"
					"<hr><center style=\"color:white\">okushnir and amyroshn webserv</center>\n</body>\n</html>");
	location.setErrorPages(status_code, s);
	return s;
}

// DELETION

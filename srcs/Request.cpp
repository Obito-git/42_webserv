#include "../inc/Request.hpp"


/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/

Request::Request(): _method(INIT), _url(""), _http_version(""),
_message(std::vector<std::string>()),
_header(std::map<std::string, std::string>()), _host(""), _content_length(""),
_server(NULL), _location(NULL), _ws(std::vector<const Server*>()),
_mime(NULL)	{};


Request::Request(const char *message,  const ClientSocket *sock,  const std::map<std::string, std::string> *mime):
_client_socket(sock), _method(INIT), _url(""),
_http_version(""),
_message(std::vector<std::string>()), _header(std::map<std::string, std::string>()), _host(""),
_content_length(""), _server(NULL), _location(NULL),
_ws(sock->getServers()), _mime(mime)
{
	if (message)
	{
		if (_read_message(message))
		{
			_make_map_of_headers();
			if (_fill_up_request())
				_create_response();
		}
	}
}

Request::Request(const Request &other) : _method(other._method), _url(other._url),
	_http_version(other._http_version), _message(other._message),
	_header(other._header), _host(other._host), _content_length(other._content_length),
	_server(other._server), _location(other._location), _ws(other._ws)
	 {};

// Request& Request::operator=(const Request &other) const
// {
// 	_method = other._method;
// 	_url = other._url;
// 	_http_version = other._http_version;
// 	_host = other._host;
// 	_header = other._header;
// 	_message = other._message;
// 	_content_length = other._content_length;
// 	_server = other._server;
// 	_location = other._location;
// 	_index = other._index;
// 	_ws = other._ws;
// 	return(*this);
// }

Request::~Request() {};

/******************************************************************************************************************
 ******************************************** CHECK CONFIG ********************************************************
 *****************************************************************************************************************/


int	Request::_check_server_name()
{
	std::vector<std::string> servers_name;
	std::vector<const Server *>::const_iterator it_serv = _ws.begin();
	std::vector<std::string>::iterator it_serv_nm;
	for (; it_serv != _ws.end(); ++it_serv)
	{
		servers_name = (*it_serv)->getServerName();
		for (it_serv_nm = servers_name.begin(); it_serv_nm < servers_name.end(); ++it_serv_nm)
		{
			if (!_host.empty() && ((*it_serv_nm).compare(_host) == 0))
				_server = *it_serv;
			else if (_host.empty() && ((*it_serv_nm).compare(_host) == 0))
				_server = *it_serv;
		}
	}
	if (_server == NULL)
		_server = _ws.front();
	return (1);
}

int	Request::_check_location()
{

	std::string location = _url;
	size_t pos = 0;
	if (*(_url.end() - 1) != '/')
	{
		pos = location.find_last_of('/');	
		location = location.substr(0, pos + 1);
	}
	while (location.length() != 1)
	{
		std::map<std::string, Location> map_loc = _server->getLocations();
		std::map<std::string, Location>::iterator it_loc = map_loc.find(location);
		if (it_loc != map_loc.end())
		{
			_location = &(*it_loc).second;
			return (_check_methods());
		}
		else
		{
			location.erase(location.end() - 1);
			pos = location.find_last_of('/');	
			location = location.substr(0, pos + 1);
		}
	}
	_location = &(_server->getConstDefault());
	return (_check_methods());
}

int	Request::_check_methods()
{
	const std::set<HTTP_METHOD> methods = _location->getAllowedMethods();
	if (methods.empty() || methods.find(_method) != methods.end())
		return (1);
	_rep = Response::_generate_reponse_error(this, 405, "Method Not Allowed"); // header : alowd_metods
	return (0);
}

void	Request::_create_response()
{
	if (_check_server_name() && _check_location())
	{
		Response response(this);
		_rep = response.getResponse();
	}
}

/******************************************************************************************************************
 ******************************************** CHECK REQUEST *******************************************************
 *****************************************************************************************************************/

int	Request::_check_url(std::vector<std::string> line)
{
	std::string element = line[1];

	if (element[0] != '/')
	{
		_rep = Response::_generate_reponse_error(this, 400, "Bad Request3");
		return (1);
	}
	size_t pos = element.find(".");
	size_t pos1 = element.find_last_of(".");
	if (pos != pos1)
	{
		_rep = Response::_generate_reponse_error(this, 400, "Bad Request4");
		return (1);
	}
	return (0);
}

int	Request::_check_first_line()
{
	std::vector<std::string> line = *(ft_split(_message[0], ' '));

	size_t pos = (_message[0]).find(" ");
	if (pos == 0 || pos == std::string::npos || line.size() != 3)
	{
		_rep = Response::_generate_reponse_error(this, 400, "Bad Request");
		return (1);
	}
	if ((line.front()).compare("GET") && (line.front()).compare("POST")
		&& (line.front()).compare("DELETE"))
	{
		_rep = Response::_generate_reponse_error(this, 405, "Method Not Allowed");
		return (1);
	}
	
	if ((line.back()).compare("HTTP/1.0") && (line.back()).compare("HTTP/1.1") &&
			(line.back()).compare("HTTP/2") && (line.back()).compare("HTTP/3"))
	{
		_rep = Response::_generate_reponse_error(this, 400, "Bad Request");
		return (1);
	}
	return (_check_url(line));
}

int	Request::_check_second_line()
{
	if (_message.size() > 1 && (_message[1][0] == '\t' || _message[1][0] == 0 //FIXME condition
		|| _message[1][0] == '\v' || _message[1][0] == '\f'
		|| _message[1][0] == '\r' || _message[1][0] == ' '))
	{
		_rep = Response::_generate_reponse_error(this, 400, "Bad Request");
		return (1);
	}
	return (0);
}

void	Request::_check_line(std::string line)
{
	if (!line.empty())
	{
		if (line.back() == '\r')
			line.erase(line.end() - 1);
		_message.push_back(line);
	}
}

/******************************************************************************************************************
 ******************************************** PARSING *************************************************************
 *****************************************************************************************************************/

int	Request::_read_message(const char * message)
{
	std::istringstream text(message);
	std::string		line;

	std::getline(text, line);
	_check_line(line);
	if (!_check_first_line())
	{
		std::getline(text, line);
		_check_line(line);
		if (!_check_second_line()) // FIXME
		{
			while (!text.eof())
			{
				std::getline(text, line);
				_check_line(line);
			}
			return (1);
		}
	}
	return (0);
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

/******************************************************************************************************************
 ******************************************** FILL_UP_REQUEST *****************************************************
 *****************************************************************************************************************/

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

void	Request::_fill_up_url(std::string element)
{
	_url = element;
	size_t	pos = _url.find("?");
	if (pos != std::string::npos)
	{
		_query = _url.substr(pos);
		_url = _url.substr(0,pos);
		// std::cout << "_query: " << _query << std::endl;
		// std::cout << "_url after _query: " << _url << std::endl;
	}
	size_t pos_point = _url.find(".");
	size_t pos_slesh = std::string::npos;
	if (pos_point != std::string::npos)
	{
		pos_slesh = _url.find('/', pos_point);
		_extention = _url.substr(pos_point + 1, pos_slesh - pos_point - 1);
		// std::cout << "_extention: " << _extention << std::endl;
		if (pos_slesh != std::string::npos)
		{
			_path_info = _url.substr(pos_slesh);
			// std::cout << "_path_info: " << _path_info << std::endl;
			_url = _url.substr(0,pos_slesh);
			// std::cout << "_url after _path_info: " << _url << std::endl;
		}
	}
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

int	Request::_fill_up_content_length()
{
	std::map<std::string, std::string>::iterator it;
	it = _header.find("Content-Length");
	if (it != _header.end())
		this->_content_length = (*it).second;
	else
	{
		_rep = Response::_generate_reponse_error(this, 411, "Length Required");
		return (0);
	}
	return (1);
}

int	Request::_fill_up_request()
{
	std::vector<std::string> line = *(ft_split(_message[0], ' '));
	_fill_up_method(line[0]);
	_fill_up_url(line[1]);
	_fill_up_protocol(line[2]);
	std::map<std::string, std::string>::iterator it;
	it = _header.find("Host");
	if (it != _header.end())
		_fill_up_host(it);
	// it = _header.find("Accept");
	// if (it != _header.end())
	// 	_content_type = "text/html";
		//this->_content_type = (*it).second;//FIXME à faire une new fonction
	if (_method == POST)
	{
		return (_fill_up_content_length());
	}
	return (1);
}

/******************************************************************************************************************
 ******************************************** PRINTING ************************************************************
 *****************************************************************************************************************/

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

void Request::_print_mime(std::map<std::string, std::string> mimes)
{
	std::map<std::string, std::string>::iterator first = mimes.begin();
	for (; first != mimes.end(); first++)
	{
		std::cout << "key : " <<(*first).first << " value : " << (*first).second << std::endl;
	}
}


const Location*	Request::getLocation() const
{
	return (_location);
}

const std::string	Request::getUrl() const
{
	return (_url);
}
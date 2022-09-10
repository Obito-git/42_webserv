#include "../inc/Request.hpp"


/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/

Request::Request(): _request_body(std::string()), _method(INIT), _url(""), _http_version(""),
_message(std::vector<std::string>()),
_header(std::map<std::string, std::string>()), _host(""), _content_length(""),
_server(NULL), _location(NULL), _ws(std::vector<const Server*>()),
_mime(NULL)	{};


Request::Request(const char *message,  const ClientSocket *sock,  const std::map<std::string, std::string> *mime):
_request_body(std::string()), _client_socket(sock), _method(INIT), _url(""),
_http_version(""),
_message(std::vector<std::string>()), _header(std::map<std::string, std::string>()), _host(""),
_content_length(""), _server(NULL), _location(NULL),
_ws(sock->getServers()), _mime(mime)
{
	if (message)
	{
		if (_read_message(message))
		{
			_read_body(message);
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
	if (*(location.end() - 1) == '/' && location.length() != 1)
		location = location.erase(location.size() - 1);
	while (location.length() > 1)
	{
		const std::map<std::string, Location> *map_loc = &(_server->getLocations());
		std::map<std::string, Location>::const_iterator it_loc = map_loc->find(location);
		if (it_loc != (*map_loc).end())
		{
			_location = &(*it_loc).second;
			return (_check_methods());
		}
		else
		{
			// location.erase(location.end() - 1);
			pos = location.find_last_of('/');	
			location = location.substr(0, pos);
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
	_rep = Response::_generate_reponse_error(this, 405); // header : alowd_metods
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
		_rep = Response::_generate_reponse_error(this, 400);
		return (1);
	}
	// size_t pos = element.find(".");
	// size_t pos1 = element.find_last_of(".");
	// if (pos != pos1)
	// {
	// 	_rep = Response::_generate_reponse_error(this, 400);
	// 	return (1);
	// }
	return (0);
}

int	Request::_check_first_line()
{
	std::vector<std::string> line = ft_split(_message[0], ' ');//FIXME ft_split avec str

	size_t pos = (_message[0]).find(" ");
	if (pos == 0 || pos == std::string::npos || line.size() != 3)
	{
		_rep = Response::_generate_reponse_error(this, 400);
		return (1);
	}
	if ((line.front()).compare("GET") && (line.front()).compare("POST")
		&& (line.front()).compare("DELETE"))
	{
		_rep = Response::_generate_reponse_error(this, 405);
		return (1);
	}
	
	if ((line.back()).compare("HTTP/1.0") && (line.back()).compare("HTTP/1.1") &&
			(line.back()).compare("HTTP/2") && (line.back()).compare("HTTP/3"))
	{
		std::cout<< line.back()<< "|\n";
		_rep = Response::_generate_reponse_error(this, 400);
		return (1);
	}
	return (_check_url(line));
}

int	Request::_check_second_line()
{
	if (_message.size() > 2 && !_message[2].empty() && (_message[1][0] == '\t' || _message[1][0] == 0 //FIXME condition
		|| _message[1][0] == '\v' || _message[1][0] == '\f'
		|| _message[1][0] == '\r' || _message[1][0] == ' '))
	{
		_rep = Response::_generate_reponse_error(this, 400);
		return (1);
	}
	return (0);
}

void	Request::_check_line(int ind)
{
	if (_message.size() > 1 && !_message[ind].empty())
	{
		if (*(_message[ind].end() - 1) == '\r')
			_message[ind].erase(_message[ind].end() - 1);
	}
}

/******************************************************************************************************************
 ******************************************** PARSING *************************************************************
 *****************************************************************************************************************/

int	Request::_read_message(const char * message)
{
	_message = ft_split(message, '\n');
	_check_line(0);
	if (!_check_first_line())
	{
		_check_line(1);
		if (!_check_second_line())
		{
			for (size_t i = 2; i < _message.size(); ++i)
				_check_line(i);
			return (1);
		}
	}
	return (0);
}

void	Request::_read_body(const char * message)
{
	std::string str(message);
	size_t pos = str.find("\r\n\r\n");
	if (pos != std::string::npos)
		if ((pos + 4 ) < (str.size() - 1)) {
			_request_body = str.substr(pos + 4);
			return;
		}
	pos = str.find("\n\n");
	if (pos != std::string::npos)
		if ((pos + 2) < (str.size() - 1))
			_request_body = str.substr(pos + 2);
}

// MAKE MAP OF HEADERS

void	Request::_make_map_of_headers()
{
	std::string key;
	std::string value;
	size_t pos_colon = 0;
	size_t pos_value = 0;

	std::vector<std::string>::iterator it = _message.begin();
	it++;
	for(; it != _message.end() && it->length() > 0 && it->at(0) != '\n'; ++it)
	{
		pos_colon = (*it).find(":");
		if (pos_colon != std::string::npos && (*it)[pos_colon - 1] != ' ')
		{
			key = (*it).substr(0, pos_colon);
			pos_value = (*it).find_first_not_of(" \t\v", pos_colon + 1);
			if (pos_value != std::string::npos)
			{
				value = (*it).substr(pos_value);
				_header.insert(std::make_pair(key, value));
			}
		}
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
	}
	size_t pos_point = _url.find_last_of(".");
	size_t pos_slesh = std::string::npos;
	if (pos_point != std::string::npos)
	{
		pos_slesh = _url.find('/', pos_point);
		_extention = _url.substr(pos_point + 1, pos_slesh - pos_point - 1);
		if (pos_slesh != std::string::npos)
		{
			_path_info = _url.substr(pos_slesh);
			_url = _url.substr(0,pos_slesh);
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
		_rep = Response::_generate_reponse_error(this, 411);
		return (0);
	}
	return (1);
}

// void	Request::_fill_up_body()
// {
// 	std::vector<std::string>::iterator it = _message.begin();
// 	for (; it < _message.end(); ++it)
// 	{
// 		if ((*it).empty())
// 			break ;
// 	}
// 	it++;
// 	if (it != _message.end())
// 	{
// 		for (; it != _message.end(); ++it)
// 		{
// 			_request_body += (*it);
// 			if ((it + 1) != _message.end())
// 				_request_body += "\n";
// 		}
// 	}
// }

int	Request::_check_body()
{
	size_t	int_content_length = atoi(_content_length.c_str());
	size_t	real_size = _request_body.size();
	if (int_content_length != real_size)
	{
		_rep = Response::_generate_reponse_error(this, 400);//FIXME is this error?
		return (0);
	}
	/*
	if (real_size > _location->getMaxBodySize())
	{
		_rep = Response::_generate_reponse_error(this, 413);//FIXME is this error?
		return (0);
	}*/
	return (1);
}

int	Request::_fill_up_request()
{
	std::vector<std::string> line = ft_split(_message[0], ' ');
	_fill_up_method(line[0]);
	_fill_up_url(line[1]);
	_fill_up_protocol(line[2]);
	// _fill_up_body();
	std::map<std::string, std::string>::iterator it;
	it = _header.find("Host");
	if (it != _header.end())
		_fill_up_host(it);
	// it = _header.find("Accept");
	// if (it != _header.end())
	// 	_content_type = "text/html";
		//this->_content_type = (*it).second;//FIXME à faire une new fonction
	if (_method == POST) //FIXME _content_length n'est pas toujours obligatoir
	{
		return (_fill_up_content_length() && _check_body());
	}
	// else FIXME 
	// {
	// 	if (!_request_body.empty())
	// 	{
	// 		_rep = Response::_generate_reponse_error(this, 400); //FIXME code d'erreur
	// 		return (0);
	// 	}
	// }
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

const std::string Request::getExtention() const
{
	return (_extention);
}

void Request::setExtention(std::string extention) 
{
	_extention = extention;
}

void Request::setPathToFile(std::string path)
{
	_path_to_requested_file = path;
}
#include "Response.hpp"


Response::Response() : _request(NULL), _location(NULL),
_response(""), _index(std::set<std::string>()), _path(""), _content_type("") {};

Response::Response(const Request *request): _request(request), _location(request->getLocation()),
_response(""), _index(_location->getIndex()), _path(""), _content_type("")
{
    _path = _concatenate_path();
    if (_path[_path.length() - 1] == '/' && _index.size() != 0) // FIXME apres ajouter condition si index vide pour auto index
        _path_is_to_folder(_path);
    else
        _path_is_to_file(_path);
};

Response::~Response(){};


void	Response::_path_is_to_file(std::string path)
{
	try
	{
		std::string code_page = ft_read_file(path);
		if (_find_content_type(path))
			_response = _generate_reponse_ok(200, code_page);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		_response = _generate_reponse_error(_request, 404);
	}
}

void	Response::_path_is_to_folder(std::string path)
{
	std::set <std::string>::iterator it_index = _index.begin();
	for (; it_index != _index.end(); ++it_index)
	{
		std::string tmp_path = path;
		tmp_path += (*it_index);
		try
		{
			std::string code_page = ft_read_file(tmp_path);
			_find_content_type(*it_index);
			_response = _generate_reponse_ok(200, code_page);
			break ;
		}
		catch (std::exception& e) {}
		_response = _generate_reponse_error(_request, 404);
	}
}//FIXMI s'il n'y a pas de index

int	Response::_find_content_type(std::string filename)
{
	std::string extention;
	size_t pos = filename.find_last_of(".");
	extention = (filename.substr(pos + 1));
	std::map<std::string, std::string>::const_iterator it = _request->_mime->find(extention);
	if (it != _request->_mime->end())
	{
		_content_type = it->second;
		return (1);
	}
	else
	{
		CGI_Handler cgi(_request);
		int status = cgi.getStatus();
		if (status == 200)
			_response = _generate_reponse_cgi(cgi, status);
		else
			_generate_reponse_error(_request, status);
		return (0);
	}
}

std::string	Response::_generate_reponse_cgi(CGI_Handler cgi, int status)
{
	std::stringstream buf;
	std::string code_page = cgi.getResult();

	// size_t size = code_page.length();

	std::time_t now = time(0);
	tm *gmtm = gmtime(&now);
	char* date = asctime(gmtm);

	buf << "HTTP/1.1 " << status << " " << HttpStatus::reasonPhrase(status) << std::endl;
	buf << "Date: " << date;
	buf << "Server:" << "Webserver" << std::endl;
	buf << code_page << std::endl << std::endl;
	return (buf.str());
}


std::string	Response::_concatenate_path()
{

	std::string path = _location->getRoot();
		return path.append(_request->getUrl());
}

std::string	Response::_generate_reponse_ok(int code, std::string code_page)
{
	std::stringstream buf;
	
	size_t size = code_page.length();
	buf << _generate_reponse_headers(code, size); //FIXME
	buf << code_page << std::endl << std::endl;
	return (buf.str());
}

std::string	Response::_generate_reponse_error(const Request *request, int code)
{
	std::stringstream buf;
	std::string code_status = HttpStatus::reasonPhrase(code);

	std::string body = _generate_error_body(request->getLocation(), code);

	buf << _generate_reponse_headers(code, body.length());
	buf << body;
	return (buf.str());
}

std::string Response::_generate_error_body(const Location *location, short status_code)
{
	std::string s = "<html>\n<head><title>";
	std::stringstream code;
	code << status_code << " " << HttpStatus::reasonPhrase(status_code);

	s.append(code.str());
	s.append("</title></head>\n<body bgcolor=\"black\">\n<p><img style=\"display: block; margin-left: auto;"
			 "margin-right: auto;\"src=\"https://http.cat/").append(code.str().substr(0,code.str().find(' ')));
	s.append("\" alt=\"");
	s.append(code.str()).append(" width=\"750\" height=\"520\"/></p>\n"
					"<hr><center style=\"color:white\">okushnir and amyroshn webserv</center>\n</body>\n</html>\n");

	if (location != NULL)
	{
		const std::map<short, std::string>& error_pages = (*location).getErrorPages();
		std::map<short, std::string>::const_iterator it = error_pages.find(status_code);
		if (it != error_pages.end())
			return (*it).second;
		// else
		// 	(*location).setErrorPages(status_code, s);
	}
	return s;
}

std::string	Response::_generate_reponse_headers(int code, size_t size)
{
	std::stringstream buf;
	std::string code_status = HttpStatus::reasonPhrase(code);

	std::time_t now = time(0);
	tm *gmtm = gmtime(&now);
	char* date = asctime(gmtm);

	buf << "HTTP/1.1 " << code << " " << code_status << std::endl;
	buf << "Date: " << date;
	buf << "Server:" << "Webserver" << std::endl;
    // if (_content_type != "")
	    // buf << "Content-Type: " << _content_type << std::endl;
	buf << "Content-Length: " << size << std::endl << std::endl;
	return (buf.str());
}


// GETTERS

const std::string Response::getResponse() const
{
    return (_response);
}
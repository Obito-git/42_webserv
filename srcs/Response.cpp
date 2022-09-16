#include "Response.hpp"


Response::Response() : _request(NULL), _location(NULL),
_response(""), _index(std::set<std::string>()), _path(""), _content_type("") {};

Response::Response(Request *request): _request(request), _location(request->getLocation()),
_response(""), _index(_location->getIndex()), _path(""), _content_type("")
{
	std::ifstream in;
	std::string file_content;
	struct stat s;
	

    _path = _concatenate_path();
	if (_request->getMethod() == DELETE)
		_delete_method();
	else
	{
		stat(_path.data(), &s);
		if (!_is_file(_path))
			_path_is_to_folder(_path);
		else
			_path_is_to_file(_path);
	}
};

Response::~Response(){};


void	Response::_path_is_to_file(std::string path)
{
	try
	{
		std::string code_page = ft_read_file(path);
		_request->setPathToFile(path);
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
			_request->setPathToFile(tmp_path);
			if (_find_content_type(*it_index))
				_response = _generate_reponse_ok(200, code_page);
			return ;
		}
		catch (std::exception& e) {}
	}
	if (_request->getLocation()->isAutoindex())
	{
		std::string auto_index_rep = AutoIndex::generate_autoindex_page(path, _request);
		if (auto_index_rep != "")
			_response = _generate_reponse_ok(200, auto_index_rep);
		else
			_response = _generate_reponse_error(_request, 403);
	}
	else
	{
		_response = _generate_reponse_error(_request, 403);
	}
}

int	Response::_find_content_type(std::string filename)
{
	std::string extention;
	size_t pos = filename.find_last_of(".");
	extention = (filename.substr(pos + 1));
	_request->setExtention(extention);
	// _request->_extention = extention;
	std::map<std::string, std::string>::const_iterator it = _request->getMime()->find(extention);
	if (it != _request->getMime()->end())
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
			_response = _generate_reponse_error(_request, status);
		return (0);
	}
}

std::string	Response::_generate_reponse_redirection(Request *request, int code)
{
	std::stringstream buf;
	std::string code_status = HttpStatus::reasonPhrase(code);

	std::string body = _generate_redirection_body(request, code);

	buf << _generate_reponse_headers(request, code, body.length());
	buf << body;
	return (buf.str());	
}


std::string	Response::_generate_reponse_cgi(const CGI_Handler &cgi, int status)
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
	buf << _generate_reponse_headers(_request, code, size); //FIXME
	buf << code_page << std::endl << std::endl;
	return (buf.str());
}

std::string	Response::_generate_reponse_error(Request *request, int code)
{
	std::stringstream buf;
	std::string code_status = HttpStatus::reasonPhrase(code);

	std::string body = _generate_error_body(request->getLocation(), code);

	buf << _generate_reponse_headers(request, code, body.length());
	buf << body;
	return (buf.str());
}

std::string Response::_generate_error_body(const Location *location, short status_code)
{
	std::string s = "<html>\n<head><title>";
	std::stringstream code;
	code << status_code << " " << HttpStatus::reasonPhrase(status_code);

	s.append(code.str());
	s.append("</title></head>\n<body bgcolor=\"black\">\n<center style=\"color:white\">");
	s.append(code.str());
	s.append("</center>");
	s.append("\n<p><img style=\"display: block; margin-left: auto;"
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

std::string Response::_generate_redirection_body(Request *request, short status_code)
{
	std::string s = "<html>\n<head><title>";
	std::stringstream code;
	code << status_code << " " << HttpStatus::reasonPhrase(status_code);

	std::cout << "request->getRedirection(): " << request->getRedirection() << std::endl;
	s.append(code.str());
	s.append("</title></head>\n<body bgcolor=\"black\">\n<p><img style=\"display: block; margin-left: auto;"
			 "margin-right: auto;\"src=\"https://http.cat/").append("301\"");
	s.append("width=\"750\" height=\"520\"/></p>");
	s.append("<h1 color=\"yellow\">301 Moved</h1><h3><center style=\"color:white\"> The document has moved <a href=\"" + request->getRedirection() + "\">here</a>.</center>");
	s.append("<hr><center style=\"color:white\">okushnir and amyroshn webserv</center>\n</body>\n</html>\n");
	return s;
}

std::string	Response::_generate_reponse_headers(Request *request, int code, size_t size)
{
	std::stringstream buf;
	std::string code_status = HttpStatus::reasonPhrase(code);

	std::time_t now = time(0);
	tm *gmtm = gmtime(&now);
	char* date = asctime(gmtm);
	
	
	buf << "HTTP/1.1 " << code << " " << code_status << std::endl;
	buf << "Date: " << date;
	buf << "Server:" << " Webserver" << std::endl;
	if (code == 301)
		buf << "Location: " << request->getRedirection() << std::endl;
    // if (_content_type != "")
	    // buf << "Content-Type: " << _content_type << std::endl;
	if (request->getHeader().find("Connection") != request->getHeader().end())
		buf << request->getHeader().find("Connection")->second << std::endl;//FIXME anton changes
	buf << "Content-Length: " << size << std::endl << std::endl;
	return (buf.str());
}


// GETTERS

const std::string Response::getResponse() const
{
    return (_response);
}


bool Response::_is_file(const std::string &path)
{
	struct stat path_stat;
	stat(path.data(), &path_stat);
	if(!S_ISREG(path_stat.st_mode) && path.find_last_of('.') != std::string::npos)
		return true;
	return S_ISREG(path_stat.st_mode);
}

void	Response::_delete_method()
{
	_response = "";
	if (_is_file(_path))
	{
		if (remove(_path.c_str()) == 0)
		{
			_response = _generate_reponse_ok(204, "");
		}
		else
		{
			_response = _generate_reponse_error(_request, 403);
		}
	}
	else
		_response = _generate_reponse_error(_request, 404);
}
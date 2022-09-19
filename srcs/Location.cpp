//
// Created by amyroshn on 8/16/22.
//

#include "Location.hpp"
const std::string Location::_location_keywords[MAX_SERV_KEYWORDS] = {"error_page", "client_max_body_size",
																	 "file_upload", "methods", "index",
																	 "autoindex", "root", "return",
																	 "cgi_path"};

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/
 
Location::Location(): _location(), _root(), _autoindex(false), _index(), _upload_path(),
                      _max_body_size(1024), _error_pages(), _cgi_path(), _allowed_methods(), _redirections() {
	_allowed_methods.insert(GET);
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/


const std::string &Location::getLocation() const {
	return _location;
}

const std::string &Location::getRoot() const {
	return _root;
}
bool Location::isAutoindex() const {
	return _autoindex;
}
const std::set<std::string> &Location::getIndex() const {
	return _index;
}

unsigned long Location::getMaxBodySize() const {
	return _max_body_size;
}

const std::set<HTTP_METHOD> &Location::getAllowedMethods() const {
	return _allowed_methods;
}
const std::map<short, std::string> &Location::getErrorPages() const {
	return _error_pages;
}

const std::vector<std::string> &Location::getCgiPath() const {
	return _cgi_path;
}
const std::map<std::string, std::string> &Location::getRedirections() const {
	return _redirections;
}


/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Location::setRoot(const std::string &root) {
	_root = root;
}
void Location::setLocation(const std::string &location) {
	_location = location;
	if (location.size() > 1 && *(_location.end() - 1) == '/')
		_location.erase(_location.end() - 1);
}

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Location::setIndex(const std::string &index) {
	_index.insert(index);
}

void Location::setMaxBodySize(unsigned long maxBodySize) {
	_max_body_size = maxBodySize;
}

void Location::setAllowedMethods(const HTTP_METHOD allowedMethods) {
	_allowed_methods.insert(allowedMethods);
}

void Location::setErrorPages(short status_code, const std::string &errorPage) {
	_error_pages.erase(status_code);
	_error_pages.insert(std::make_pair(status_code, errorPage));
}

void Location::setCgiPath(const std::vector<std::string> &cgiPath) {
	_cgi_path = cgiPath;
}

void Location::setRedirections(const std::string& k, const std::string &v) {
	_redirections.insert(std::make_pair(*(k.end() - 1) == '/' ? k.substr(0, k.size() - 1): k, v));
}


//--------------------------------------------------------------

Location::Location(const Location& other) {
	_location = other._location;
	_root = other._root;
	_autoindex = other._autoindex;
	_index = other._index;
	_upload_path = other._upload_path;
	_max_body_size = other._max_body_size;
	_redirections = other._redirections;
	_error_pages = other._error_pages;
	_cgi_path = other._cgi_path;
	_allowed_methods = other._allowed_methods;
}

Location &Location::operator=(const Location& other){
	if (&other == this)
		return *this;
	_location = other._location;
	_root = other._root;
	_autoindex = other._autoindex;
	_index = other._index;
	_upload_path = other._upload_path;
	_max_body_size = other._max_body_size;
	_error_pages = other._error_pages;
	_cgi_path = other._cgi_path;
	_allowed_methods = other._allowed_methods;	
	_redirections = other._redirections;
	return *this;
}

const std::string &Location::getUploadPath() const {
	return _upload_path;
}

void Location::setUploadPath(const std::string &uploadPath) {
	_upload_path = uploadPath;
}



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
 
Location::Location() {
	_autoindex = false;
	_file_upload = false;
	_max_body_size = 1024;
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

bool Location::isFileUpload() const {
	return _file_upload;
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

const std::pair<std::string, std::string> &Location::getRedirectTo() const {
	return _redirect_to;
}

const std::vector<std::string> &Location::getCgiPath() const {
	return _cgi_path;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Location::setRoot(const std::string &root) {
	_root = root;
}
void Location::setLocation(const std::string &location) {
	_location = location;
	if (*(_location.end() - 1) == '/')
		_location.erase(_location.end() - 1);
}

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Location::setIndex(const std::string &index) {
	_index.insert(index);
}

void Location::setFileUpload(bool fileUpload) {
	_file_upload = fileUpload;
}

void Location::setMaxBodySize(unsigned long maxBodySize) {
	_max_body_size = maxBodySize;
}

void Location::setAllowedMethods(const HTTP_METHOD allowedMethods) {
	_allowed_methods.insert(allowedMethods);
}

void Location::setErrorPages(short status_code, const std::string &errorPage) {
	_error_pages.insert(std::make_pair(status_code, errorPage));
}

void Location::setRedirectTo(const std::pair<std::string, std::string> &redirectTo) {
	_redirect_to = redirectTo;
}

void Location::setCgiPath(const std::vector<std::string> &cgiPath) {
	_cgi_path = cgiPath;
}



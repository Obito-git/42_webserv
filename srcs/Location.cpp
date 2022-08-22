//
// Created by amyroshn on 8/16/22.
//

#include "Location.hpp"
/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/
 
Location::Location() {
	_autoindex = false;
	_file_upload = false;
	_max_body_size = 1024;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
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
const std::map<short, std::pair<std::string, std::string> > &Location::getErrorPages() const {
	return _error_pages;
}
const std::string &Location::getRedirectFrom() const {
	return _redirect_from;
}

const std::pair<std::string, std::string> &Location::getRedirectTo() const {
	return _redirect_to;
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

const std::vector<std::string> &Location::getCgiPath() const {
	return _cgi_path;
}

void Location::setRoot(const std::string &root) {
	_root = root;
}
void Location::setLocation(const std::string &location) {
	_location = location;
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

void Location::setErrorPages(short status_code, const std::pair<std::string, std::string> &errorPage) {
	_error_pages.insert(std::make_pair(status_code, errorPage));
}

void Location::setRedirectFrom(const std::string &redirectFrom) {
	_redirect_from = redirectFrom;
}

void Location::setRedirectTo(const std::pair<std::string, std::string> &redirectTo) {
	_redirect_to = redirectTo;
}

void Location::setCgiPath(const std::vector<std::string> &cgiPath) {
	_cgi_path = cgiPath;
}



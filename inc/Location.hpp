//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP
#include "webserv.hpp"

struct Location {
private:
	std::string												_location;
	std::string												_root;
	bool													_autoindex;
	std::set <std::string>									_index;
	bool													_file_upload;
	unsigned long											_max_body_size;
	std::string 											_redirect_from;
	std::pair<std::string, std::string>						_redirect_to; //<filename, file content>
	std::map<short, std::pair<std::string, std::string>	>	_error_pages;
	std::vector<std::string>								_cgi_path;
	std::set<HTTP_METHOD>									_allowed_methods;
public:
	//constructor
	Location();

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	const std::string &getRoot() const;
	bool isAutoindex() const;
	const std::set<std::string> &getIndex() const;
	bool isFileUpload() const;
	unsigned long getMaxBodySize() const;
	const std::map<short, std::pair<std::string, std::string> > &getErrorPages() const;
	const std::set<HTTP_METHOD> &getAllowedMethods() const;
	const std::vector<std::string> &getCgiPath() const;
	const std::string &getLocation() const;
	const std::string &getRedirectFrom() const;
	const std::pair<std::string, std::string> &getRedirectTo() const;

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/
 
	void setLocation(const std::string &location);
	void setRoot(const std::string &root);
	void setAutoindex(bool autoindex);
	void setIndex(const std::string &index);
	void setFileUpload(bool fileUpload);
	void setMaxBodySize(unsigned long maxBodySize);
	void setErrorPages(short status_code, const std::pair<std::string, std::string> &errorPage);
	void setAllowedMethods(HTTP_METHOD allowedMethods);
	void setCgiPath(const std::vector<std::string> &cgiPath);
	void setRedirectFrom(const std::string &redirectFrom);
	void setRedirectTo(const std::pair<std::string, std::string> &redirectTo);

};


#endif //WEBSERV_LOCATION_HPP

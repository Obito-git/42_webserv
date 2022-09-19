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
	std::string												_upload_path;
	unsigned long											_max_body_size;
	std::map<short, std::string>							_error_pages;
	std::vector<std::string>								_cgi_path;
	std::set<HTTP_METHOD>									_allowed_methods;
	std::map<std::string, std::string>						_redirections;
	
public:
	//constructor
	Location();
	Location(const Location& other);
	Location& operator=(const Location& other);
	static const std::string    _location_keywords[];

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	const std::string &getRoot() const;
	bool isAutoindex() const;
	const std::set<std::string> &getIndex() const;
	unsigned long getMaxBodySize() const;
	const std::map<short, std::string> &getErrorPages() const;
	const std::set<HTTP_METHOD> &getAllowedMethods() const;
	const std::vector<std::string> &getCgiPath() const;
	const std::string &getLocation() const;
	const std::map<std::string, std::string> &getRedirections() const;

	

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/
 
	void setLocation(const std::string &location);
	void setRoot(const std::string &root);
	void setAutoindex(bool autoindex);
	void setIndex(const std::string &index);
	void setMaxBodySize(unsigned long maxBodySize);
	void setErrorPages(short status_code, const std::string &errorPage);
	void setAllowedMethods(HTTP_METHOD allowedMethods);
	void setCgiPath(const std::vector<std::string> &cgiPath);
	void setRedirections(const std::string& k, const std::string &v);

	const std::string &getUploadPath() const;

	void setUploadPath(const std::string &uploadPath);
};


#endif //WEBSERV_LOCATION_HPP

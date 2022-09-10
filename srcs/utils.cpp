//
// Created by Anton on 16/08/2022.
//

#include "webserv.hpp"

std::vector<std::string> ft_split(std::string s, char delim) {
	std::vector<std::string> tmp_res;

	size_t pos = 0;
	while (( pos = s.find (delim)) != std::string::npos)
	{
		if (!s.substr(0, pos).empty())
			tmp_res.push_back(s.substr(0, pos)); // store the substring
		s.erase(0, pos + 1);  /* erase() function store the current positon and move to next token. */
	}
	if (!s.substr(0, pos).empty())
		tmp_res.push_back(s.substr(0, pos)); // store the substring
	return tmp_res;
}

std::vector<std::string> ft_split(std::string s, const std::string &delim) {
	std::vector<std::string> tmp_res;

	size_t pos = 0;
	while (( pos = s.find (delim)) != std::string::npos)
	{
		if (!s.substr(0, pos).empty())
			tmp_res.push_back(s.substr(0, pos)); // store the substring
		s.erase(0, pos + delim.length());  /* erase() function store the current positon and move to next token. */
	}
	if (!s.substr(0, pos).empty())
		tmp_res.push_back(s.substr(0, pos)); // store the substring
	return tmp_res;
}

std::string ft_read_file(const std::string& path) {
	std::ifstream in;
	std::string file_content;
	struct stat s;
	bool error = false;
	if (stat(path.data(), &s) == 0) {
		if (s.st_mode & S_IFDIR)
			error = true;
		else if(!(s.st_mode & S_IFREG))
			error = true;
	} else
		error = true;

	//trying to open file and checking elementary errors
	if (!error)
		in.open(path.data(), std::ios::in);
	if (!in.is_open())
		throw std::runtime_error(std::string("Cannot open ") + path);

	std::string tmp;
	while (getline(in, tmp))
			file_content.append(tmp).append("\n");
	in.close();
	return file_content;
}

std::string get_method_name(HTTP_METHOD m) {
	switch (m) {
		case GET: return "GET";
		case POST: return "POST";
		case DELETE: return "DELETE";
		case INIT: return "INIT";
		default: return "OTHER";
	}
}

std::string ft_to_upper_case(const std::string &s) {
	std::string res(s);
	for (size_t i = 0; i < res.length(); i++)
		if (islower(res[i]))
			res[i] = toupper(res[i]);
	return res;
}

bool ft_isnumeric(const char *s) {
	for (int i = 0; s[i]; i++)
		if (!isdigit(s[i])) { return false; }
	return true;
}


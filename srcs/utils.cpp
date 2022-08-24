//
// Created by Anton on 16/08/2022.
//

#include "webserv.hpp"

std::vector<std::string>* ft_split(std::string s, char delim) {
	// given string with delimiter
	std::vector<std::string>* tmp_res = new std::vector<std::string>();

	size_t pos = 0;
	while (( pos = s.find (delim)) != std::string::npos)
	{
		if (!s.substr(0, pos).empty())
			tmp_res->push_back(s.substr(0, pos)); // store the substring
		s.erase(0, pos + 1);  /* erase() function store the current positon and move to next token. */
	}
	if (!s.substr(0, pos).empty())
		tmp_res->push_back(s.substr(0, pos)); // store the substring
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
		throw std::runtime_error("Cannot open file\n");

	std::string tmp;
	while (getline(in, tmp))
			file_content.append(tmp).append("\n");
	in.close();
	return file_content;
}



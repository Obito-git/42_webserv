#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>



void	_make_map_of_mime(std::vector<std::string> my_vector, std::map<std::string, std::string> *my_map)
{
	std::string key;
	std::string value;
	size_t pos = 0;

	std::vector<std::string>::iterator it = my_vector.begin();
	it++;
	for(; it < my_vector.end(); ++it)
	{
		pos = (*it).find(" ", 4);
		key = (*it).substr(4, pos);
		pos = (*it).find_last_of(" "); // attention, pas toujours
		value = (*it).substr(pos + 1);
		my_map->insert(std::make_pair(key, value));
	}
}

void	_read_message(std::ifstream message, std::vector<std::string> &my_vector)
{
	std::string		line;

	std::getline(message, line);
	while (!message.eof())
	{
		std::getline(message, line);
		my_vector.push_back(line);
	}
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::vector<std::string> my_vector;
		std::map<std::string, std::string> my_map;
		std::ifstream	text;
		std::string		line;

		text.open(argv[1], std::ifstream::in);
		std::getline(text, line);
		my_vector.push_back(line);
		while (!text.eof())
		{
			std::getline(text, line);
			my_vector.push_back(line);
		}
		text.close();
		std::string key;
		std::string value;
		size_t pos = 0;

		std::vector<std::string>::iterator it = my_vector.begin();
		for(; it < my_vector.end(); ++it)
		{
			pos = (*it).find(" ");
			key = (*it).substr(0, pos);
			pos = (*it).find_last_of(" "); // attention, pas toujours
			value = (*it).substr(pos + 1);
			my_map.insert(std::make_pair(key, value));
			std::cout << value << ": " << key << std::endl;
		}

		// std::cout <<": " <<std::endl;
		// std::map<std::string, std::string>::iterator it1 = my_map.begin();
		// std::cout << my_map.empty() << std::endl;
		// // std::cout << (*it) << std::endl;
		// for(; it1 != my_map.end(); ++it)
		// {
		// 	std::cout << (*it1).first << ": " << (*it1).second << std::endl;
		// }
	}
	return (0);
}
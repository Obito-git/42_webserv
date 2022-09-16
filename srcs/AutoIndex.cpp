#include "../inc/AutoIndex.hpp"

AutoIndex::AutoIndex() {};

AutoIndex::~AutoIndex() {};

// STATIC FONCTIONS

std::string	AutoIndex::generate_autoindex_page(std::string path, Request* req)
{
    DIR *dir = opendir(path.c_str());
    std::string page =\
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" + path + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

    if (dir == NULL)
        return "";
    if (path[0] != '/')
        path = "/" + path;
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        page += _getLink(std::string(dirEntry->d_name), path, req);
    }
    page +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return page;
}

std::string AutoIndex::_getLink(std::string const &dirEntry, std::string path, Request* req)
{
    size_t pos = path.find_last_of("/");
    path = path.substr(pos);
    std::string addr = req->getHost();
    if (req->getPort() != "")
        addr += ":" + req->getPort();
    std::stringstream   ss;
    ss << "\t\t<p><h3><a  href=\"http://" + addr << path + "/" + dirEntry + "\">" + dirEntry + "</a></h3></p>\n";
    return ss.str();
}

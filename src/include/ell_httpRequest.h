#pragma once


#include <string>
#include <unordered_map>

class ell_httpRequest {
private:
    std::string url;
    std::string method;
    std::string version;
    std::unordered_map<std::string, std::string> header;
    std::string body;

    std::string header_field;
    std::string header_value;

public:
    ell_httpRequest();
    ~ell_httpRequest();

    void setURL(std::string _url);
    void setMethod(std::string _method);
    void appendHeaderField(std::string _header_field);
    void appendHeaderValue(std::string _header_value);
    void setBody(std::string _body);
};
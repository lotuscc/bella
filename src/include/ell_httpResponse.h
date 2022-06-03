#pragma once


#include <string>
#include <unordered_map>

class ell_httpResponse {
private:
    std::string version;
    std::string status;
    std::string reasonPhrase;
    std::unordered_map<std::string, std::string> header;
    std::string body;

public:
    ell_httpResponse();
    ~ell_httpResponse();

    void setVersion(std::string _version);
    void setStatus(std::string _status);
    void setReasonPhrase(std::string _reasonPhrase);
    void appendHeader(std::string _header_field, std::string _header_value);
    void setBody(std::string _body);
};
#include "ell_httpResponse.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

ell_httpResponse::ell_httpResponse() {}

ell_httpResponse::~ell_httpResponse() {}

void ell_httpResponse::setVersion(std::string _version) {
    version = std::move(_version);
}
void ell_httpResponse::setStatus(std::string _status) {
    status = std::move(_status);
}
void ell_httpResponse::setReasonPhrase(std::string _reasonPhrase) {
    reasonPhrase = std::move(_reasonPhrase);
}
void ell_httpResponse::appendHeader(std::string _header_value,
                                    std::string _header_field) {
    header.insert(
        std::make_pair(std::move(_header_field), std::move(_header_value)));
}
void ell_httpResponse::setBody(std::string _body) { body = std::move(_body); }

int ell_httpResponse::SerializeToArray(char *data, int size) {
    char *start = data;

    ::strncpy(data, this->version.c_str(), this->version.size());
    data += this->version.size();
    ::strncpy(data, " ", 1);
    data += 1;

    ::strncpy(data, this->status.c_str(), this->status.size());
    data += this->status.size();
    ::strncpy(data, " ", 1);
    data += 1;

    ::strncpy(data, this->reasonPhrase.c_str(), this->reasonPhrase.size());
    data += this->reasonPhrase.size();
    ::strncpy(data, "\r\n", 2);
    data += 2;

    // headr start
    for (auto &[field, value] : this->header) {

        ::strncpy(data, value.c_str(), value.size());
        data += value.size();
        
        ::strncpy(data, ": ", 2);
        data += 2;
        
        ::strncpy(data, field.c_str(), field.size());
        data += field.size();

        ::strncpy(data, "\r\n", 2);
        data += 2;
    }

    ::strncpy(data, "\r\n", 2);
    data += 2;

    // body start
    ::strncpy(data, this->body.c_str(), this->body.size());
    data += this->body.size();
    ::strncpy(data, "\r\n", 2);
    data += 2;

    int len = data - start;

    printf("send %d bytes \n", len);

    return len;
}
#include "ell_httpRequest.h"

ell_httpRequest::ell_httpRequest() {}

ell_httpRequest::~ell_httpRequest() {}

void ell_httpRequest::setURL(std::string _url) {
    url = std::move(_url);
}
void ell_httpRequest::setMethod(std::string _method) {
    url = std::move(_method);
}
void ell_httpRequest::appendHeaderField(std::string _header_field) {
    header_field = std::move(_header_field);
}
void ell_httpRequest::appendHeaderValue(std::string _header_value) {
    header_value = std::move(_header_value);
    header.insert(
        std::make_pair(std::move(header_field), std::move(header_value)));
}
void ell_httpRequest::setBody(std::string _body) {
    body = std::move(_body);
}
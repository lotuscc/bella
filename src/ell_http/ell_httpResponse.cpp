#include "ell_httpResponse.h"
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
void ell_httpResponse::setBody(std::string _body) {
    body = std::move(_body);
}
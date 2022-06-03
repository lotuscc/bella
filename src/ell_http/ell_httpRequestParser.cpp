
#include "ell_httpRequestParser.h"


// 定义类静态成员
ell_httpRequest ell_httpRequestParser::http; 


ell_httpRequestParser::ell_httpRequestParser() {
    llhttp_settings_init(&settings);
    llhttp_init(&parser, HTTP_REQUEST, &settings);

    settings.on_message_begin = on_message_begin;
    settings.on_url = on_url;
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;
    settings.on_body = on_body;
    settings.on_message_complete = on_message_complete;
}

ell_httpRequestParser::~ell_httpRequestParser() {
    llhttp_finish(&parser);    
}

ell_httpRequest ell_httpRequestParser::execute(const char *data, size_t len) {
    const char *request =
        "POST /index.html HTTP/1.1\r\nconnection:close\r\ncontent-length: "
        "1\r\n\r\n1\r\n\r\n";
    int request_len = strlen(request);

    // enum llhttp_errno err = llhttp_execute(&parser, data, len);
    enum llhttp_errno err = llhttp_execute(&parser, request, request_len);
    if (err != HPE_OK) {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                parser.reason);
    }
    return std::move(http);
}
#pragma once

#include "llhttp.h"
#include <functional>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <utility>

#include "ell_httpRequest.h"

class ell_httpRequestParser {
public:
#define MAX_LEN 2048

private:
    llhttp_t parser;
    llhttp_settings_t settings;

    static ell_httpRequest http;

public:
    ell_httpRequestParser();
    ~ell_httpRequestParser();

    ell_httpRequestParser(const ell_httpRequestParser &) = delete;
    ell_httpRequestParser &operator=(const ell_httpRequestParser &) = delete;

    ell_httpRequest execute(const char *data, size_t len);

private:
    static int on_message_begin(llhttp_t *parser) {
        printf("parse start\n");
        return 0;
    }

    static int on_url(llhttp_t *parser, const char *at, size_t length) {
        char url[MAX_LEN];
        strncpy(url, at, length);
        url[length] = '\0';
        printf("on_url: %s\n", url);

        std::string method(llhttp_method_name((llhttp_method_t)parser->method));
        printf("method: %s\n", method.c_str());

        http.setMethod(method);
        http.setURL(std::string(at, length));        

        return 0;
    }

    static int on_header_field(llhttp_t *parser, const char *at,
                               size_t length) {
        char header_field[MAX_LEN];
        strncpy(header_field, at, length);
        header_field[length] = '\0';
        printf("head field: %s\n", header_field);

        http.appendHeaderField(std::string(at, length));

        return 0;
    }

    static int on_header_value(llhttp_t *parser, const char *at,
                               size_t length) {
        char header_value[MAX_LEN];
        strncpy(header_value, at, length);
        header_value[length] = '\0';
        printf("head value: %s\n", header_value);

        http.appendHeaderValue(std::string(at, length));

        return 0;
    }

    static int on_headers_complete(llhttp_t *parser) {
        printf("on_headers_complete, major: %d, major: %d, keep-alive: %d, "
               "upgrade: %d\n",
               parser->http_major, parser->http_minor,
               llhttp_should_keep_alive(parser), parser->upgrade);
               
        return 0;
    }

    static int on_body(llhttp_t *parser, const char *at, size_t length) {
        char body[MAX_LEN];
        strncpy(body, at, length);
        body[length] = '\0';
        printf("on_body: %s\n", body);

        http.setBody(std::string(at, length));

        return 0;
    }

    static int on_message_complete(llhttp_t *parser) {
        printf("on_message_complete\n");
        return 0;
    }
};
#include "ell_Serves.h"
#include "ell_TcpConnector.h"


using MessageCall = ell_TcpConnector::handerMessageCall;


void ell_Servers::echo(ell::ell_message *req, ell::ell_message *resp,
                       void *next) {
    resp->set_content(req->content());
}

void ell_Servers::discard(ell::ell_message *req, ell::ell_message *resp,
                          void *next) {}

void ell_Servers::sayhello(ell::ell_message *req, ell::ell_message *resp,
                           void *next) {
    resp->set_content("hello, world");
}
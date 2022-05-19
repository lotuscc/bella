#pragma once

#include "ell_message.pb.h"

class ell_Servers {

public:
    static void echo(ell::ell_message *req, ell::ell_message *resp, void *next);
    static void sayhello(ell::ell_message *req, ell::ell_message *resp,
                         void *next);
    static void discard(ell::ell_message *req, ell::ell_message *resp,
                        void *next);
};
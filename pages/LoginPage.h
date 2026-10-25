#pragma once
// Owner: Member 1 (Project lead / Auth)
#include "../services/AuthService.h"

class LoginPage {
private:
    AuthService& authService;

public:
    explicit LoginPage(AuthService& authService);

    // Ask for name and password, call AuthService::login.
    // Returns true if the login succeeded.
    bool show();
};

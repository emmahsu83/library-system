// Owner: Member 1 (Project lead / Auth)
// Asks for a name and password and calls AuthService::login.
#include "LoginPage.h"
#include <iostream>
#include <string>

LoginPage::LoginPage(AuthService& authService)
    : authService(authService) {}

bool LoginPage::show() {
    std::string name;
    std::string password;

    std::cout << "\n------------- LOGIN -------------\n";
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    if (authService.login(name, password)) {
        std::cout << "Login successful. Welcome, " << name << "!\n";
        return true;
    }

    std::cout << "Wrong name or password.\n";
    return false;
}

#pragma once
// Owner: Member 1 (Project lead / Auth)
#include <string>
#include <vector>
#include "../models/Admin.h"
#include "../models/User.h"

enum class Role { None, Admin, User };

class AuthService {
private:
    std::vector<Admin>& admins;   // shared lists, created in main.cpp
    std::vector<User>& users;
    Role currentRole;
    Admin* currentAdmin;
    User* currentUser;

public:
    AuthService(std::vector<Admin>& admins, std::vector<User>& users);

    // Check name + password against admins first, then users.
    // Returns true and remembers who logged in if correct.
    bool login(const std::string& name, const std::string& password);

    void logout();

    bool isLoggedIn() const;
    Role getRole() const;
    Admin* getCurrentAdmin() const;   // nullptr unless an admin is logged in
    User* getCurrentUser() const;     // nullptr unless a user is logged in
};

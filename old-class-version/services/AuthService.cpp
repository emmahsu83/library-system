// Owner: Member 1 (Project lead / Auth)
// Login / logout logic. It checks the shared admin and user lists.
#include "AuthService.h"

// Constructor: remember the two shared lists, nobody is logged in yet.
AuthService::AuthService(std::vector<Admin>& admins, std::vector<User>& users)
    : admins(admins), users(users),
      currentRole(Role::None), currentAdmin(nullptr), currentUser(nullptr) {}

bool AuthService::login(const std::string& name, const std::string& password) {
    logout();   // clear any old login first

    // 1. Check the admins
    for (std::size_t i = 0; i < admins.size(); i++) {
        if (admins[i].name == name && admins[i].password == password) {
            currentRole = Role::Admin;
            currentAdmin = &admins[i];
            return true;
        }
    }

    // 2. Check the users
    for (std::size_t i = 0; i < users.size(); i++) {
        if (users[i].name == name && users[i].password == password) {
            currentRole = Role::User;
            currentUser = &users[i];
            return true;
        }
    }

    return false;   // no match
}

void AuthService::logout() {
    currentRole = Role::None;
    currentAdmin = nullptr;
    currentUser = nullptr;
}

bool AuthService::isLoggedIn() const {
    return currentRole != Role::None;
}

Role AuthService::getRole() const {
    return currentRole;
}

Admin* AuthService::getCurrentAdmin() const {
    return currentAdmin;
}

User* AuthService::getCurrentUser() const {
    return currentUser;
}

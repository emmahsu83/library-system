#pragma once
// Owner: Member 3 (Admin module)
#include <string>

class Admin {
public:
    std::string name;       // also used as the login name
    std::string address;
    std::string contact;
    std::string password;   // GROUP DECISION: needed so Login can work

    Admin();
    Admin(const std::string& name, const std::string& address,
          const std::string& contact, const std::string& password);
};

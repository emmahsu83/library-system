#pragma once
// Owner: Member 4 (User module)
#include <string>
#include <vector>
#include "Book.h"

class User {
public:
    std::string name;       // also used as the login name
    std::string address;
    std::string contact;
    std::string password;   // GROUP DECISION: needed so Login can work
    std::vector<Book> books;   // books this user has borrowed

    User();
    User(const std::string& name, const std::string& address,
         const std::string& contact, const std::string& password);

    void addBook(const Book& book);                 // add to borrowed list
    bool removeBook(const std::string& isbn);       // false if not found
    bool hasBook(const std::string& isbn) const;
};

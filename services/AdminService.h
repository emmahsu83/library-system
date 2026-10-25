#pragma once
// Owner: Member 3 (Admin module)
#include <string>
#include <vector>
#include "../models/User.h"
#include "../models/Book.h"
#include "BookService.h"

class AdminService {
private:
    std::vector<User>& users;     // shared list, created in main.cpp
    BookService& bookService;     // shared book service

public:
    AdminService(std::vector<User>& users, BookService& bookService);

    // Add a new user. Returns false if the name is already taken.
    bool createUser(const User& user);

    // Add a book to the library (uses BookService::createBook).
    bool addBooks(const Book& book);

    // Return all users.
    const std::vector<User>& findAllUsers() const;

    // Take a book back from a user: remove it from the user's list,
    // then nocAvailable++ and nocCheckOut-- in BookService.
    // Returns false if the user or book is not found, or the user does not have it.
    bool bookReturn(const std::string& userName, const std::string& isbn);
};

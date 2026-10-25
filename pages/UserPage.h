#pragma once
// Owner: Member 4 (User module)
#include "../services/AuthService.h"
#include "../services/UserService.h"
#include "../services/BookService.h"

class UserPage {
private:
    AuthService& authService;
    UserService& userService;
    BookService& bookService;

public:
    UserPage(AuthService& authService, UserService& userService,
             BookService& bookService);

    // User menu loop (view all books, search by name, borrow a book,
    // view my borrowed books, logout). Returns when the user logs out.
    void show();
};

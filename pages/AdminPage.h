#pragma once
// Owner: Member 3 (Admin module)
#include "../services/AuthService.h"
#include "../services/AdminService.h"
#include "../services/BookService.h"

class AdminPage {
private:
    AuthService& authService;
    AdminService& adminService;
    BookService& bookService;

public:
    AdminPage(AuthService& authService, AdminService& adminService,
              BookService& bookService);

    // Admin menu loop (create user, add books, list users, list books,
    // return book, logout). Returns when the admin logs out.
    void show();
};

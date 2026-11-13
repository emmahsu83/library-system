// Owner: Member 1 (Project lead / Auth)
// main.cpp creates the shared data and services, then runs the main loop.
#include <iostream>
#include <vector>
#include "models/Admin.h"
#include "models/User.h"
#include "models/Book.h"
#include "services/AuthService.h"
#include "services/AdminService.h"
#include "services/UserService.h"
#include "services/BookService.h"
#include "services/StorageService.h"
#include "pages/HomePage.h"
#include "pages/LoginPage.h"
#include "pages/AdminPage.h"
#include "pages/UserPage.h"

// Save everything to the data files (called after each logout and on exit)
static void saveAll(const std::vector<Admin>& admins,
                    const std::vector<User>& users,
                    const BookService& bookService) {
    StorageService::saveAdmins(admins);
    StorageService::saveUsers(users);
    StorageService::saveBooks(bookService.findAllBooks());
}

int main() {
    // 1. Shared data: every service uses these same objects
    std::vector<Admin> admins;
    std::vector<User> users;
    BookService bookService;

    // 2. Load saved data from the files.
    //    On the very first run there are no files yet, so the lists stay empty.
    StorageService::loadAdmins(admins);
    StorageService::loadUsers(users);

    std::vector<Book> savedBooks;
    StorageService::loadBooks(savedBooks);
    for (std::size_t i = 0; i < savedBooks.size(); i++) {
        bookService.createBook(savedBooks[i]);   // BookService keeps its own list
    }

    // 3. First run only: no admin saved yet, so create the starting admin
    //    (needed so someone can log in and create users)
    //    Demo login: name = admin, password = admin123
    if (admins.empty()) {
        admins.push_back(Admin("admin", "Main Library", "012-3456789", "admin123"));
    }

    // 4. First run only: no books saved yet, so add sample books
    if (bookService.findAllBooks().empty()) {
        bookService.createBook(Book("C++ Primer", "Stanley Lippman", "9780321714114", 3));
        bookService.createBook(Book("Clean Code", "Robert Martin", "9780132350884", 2));
        bookService.createBook(Book("Intro to Algorithms", "Thomas Cormen", "9780262033848", 1));
    }

    // 5. Services (they receive the shared data by reference)
    AuthService authService(admins, users);
    AdminService adminService(users, bookService);
    UserService userService(bookService);

    // 6. Pages (they receive the services they need)
    HomePage homePage;
    LoginPage loginPage(authService);
    AdminPage adminPage(authService, adminService, bookService);
    UserPage userPage(authService, userService, bookService);

    // 7. Main loop: Home -> Login -> Admin or User page -> back to Home
    bool running = true;
    while (running) {
        int choice = homePage.show();      // 1 = login, 0 = exit

        if (choice == 0) {
            running = false;
        } else if (choice == 1) {
            if (loginPage.show()) {        // true if login worked
                if (authService.getRole() == Role::Admin) {
                    adminPage.show();      // returns when the admin logs out
                } else if (authService.getRole() == Role::User) {
                    userPage.show();       // returns when the user logs out
                }
                saveAll(admins, users, bookService);   // save after every logout
            } else {
                std::cout << "Login failed. Please try again.\n";
            }
        }
    }

    // 8. Save one last time before the program ends
    saveAll(admins, users, bookService);

    std::cout << "Goodbye!\n";
    return 0;
}

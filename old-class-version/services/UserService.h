#pragma once
// Owner: Member 4 (User module)
#include <string>
#include "../models/User.h"
#include "BookService.h"

class UserService {
private:
    BookService& bookService;     // shared book service

public:
    explicit UserService(BookService& bookService);

    // Borrow a book: check it exists and nocAvailable > 0 and the user does not
    // already have it, then nocAvailable--, nocCheckOut++, user.addBook(...).
    // Returns false if any check fails.
    bool bookBorrow(User& user, const std::string& isbn);
};

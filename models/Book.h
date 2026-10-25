#pragma once
// Owner: Member 2 (Book module)
#include <string>

class Book {
public:
    std::string title;
    std::string author;
    std::string isbn;
    int nocAvailable;   // number of copies available now
    int nocCheckOut;    // number of copies currently checked out

    Book();
    Book(const std::string& title, const std::string& author,
         const std::string& isbn, int nocAvailable);
};

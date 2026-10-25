#pragma once
// Owner: Member 2 (Book module)
#include <string>
#include <vector>
#include "../models/Book.h"

class BookService {
private:
    std::vector<Book> books;

public:
    // Add a new book. Returns false if a book with the same ISBN already exists.
    bool createBook(const Book& book);

    // Return all books.
    const std::vector<Book>& findAllBooks() const;

    // Return all books whose title matches (or contains) the given text.
    std::vector<Book> findByName(const std::string& title) const;

    // Return a pointer to the book with this ISBN, or nullptr if not found.
    // (Extra helper: UserService and AdminService need it.)
    Book* findByIsbn(const std::string& isbn);

    // Set the number of available copies for a book. Returns false if not found.
    bool updateInventory(const std::string& isbn, int newNocAvailable);

    // Remove a book from the library. Returns false if not found.
    bool deleteInventory(const std::string& isbn);
};

// ============================================================
// File    : Member2_Book.cpp
// Module  : Book (Member 2)
// Course  : PRG2203 Programming Techniques - Coursework 3
// Author  : [YOON BON SHE OO and I26042515]
// Date    : [14/11/2026]
// Compile : g++ -std=c++11 Member2_Book.cpp -o book
// Functions: isValidIsbn, addBook, findBookByTitle
// ============================================================

#include <iostream>
#include <string>
#include <climits>
using namespace std;

// ---------- shared constants (from Member 1) ----------
const int MAX_ADMINS = 10;
const int MAX_BOOKS = 100;
const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;

// ---------- shared result codes (from Member 1) ----------
const int RESULT_OK = 0;
const int ERR_BORROWER_NOT_FOUND = 1;
const int ERR_BOOK_NOT_FOUND = 2;
const int ERR_NO_COPIES = 3;
const int ERR_LOAN_LIMIT = 4;
const int ERR_ALREADY_HAS_BOOK = 5;
const int ERR_NOT_BORROWED = 6;

// ---------- shared structures (from Member 1) ----------
struct Admin {
    string userId;
    string password;
};

struct Book {
    string title;
    string author;
    string isbn;
    int copiesAvailable;
    int copiesCheckedOut;
};

struct Borrower {
    string name;
    string address;
    string contact;
    string borrowedTitles[MAX_LOANS];
    int borrowedCount;
};

// ---------- Member 2 function prototypes ----------
bool isValidIsbn(string isbn);
bool addBook(Book books[], int &bookCount, string title, string author,
             string isbn, int copies);
int findBookByTitle(const Book books[], int bookCount, string title);

// ---------- helper function prototypes ----------
string bookTrimText(string text);
bool isValidBookText(string text);
string bookToLowerText(string text);
bool readWholeNumber(string text, int &number);

// ===================== MERGE START =====================
// Member 1 copies the code between these two marker lines into final_main.cpp

// Remove spaces and tabs from the beginning and end.
string bookTrimText(string text)
{
    int start = 0;
    int end = (int)text.length() - 1;

    while (start <= end && (text[start] == ' ' || text[start] == '\t')) {
        start++;
    }
    while (end >= start && (text[end] == ' ' || text[end] == '\t')) {
        end--;
    }

    return text.substr(start, end - start + 1);
}

// Text fields must not be empty and must not contain the file separator '|'.
bool isValidBookText(string text)
{
    text = bookTrimText(text);
    return !text.empty() && text.find('|') == string::npos;
}

// Change English capital letters to lowercase for case-insensitive searching.
string bookToLowerText(string text)
{
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] - 'A' + 'a';
        }
    }
    return text;
}

// Check that the ISBN contains exactly 10 or 13 digits.
bool isValidIsbn(string isbn)
{
    if (isbn.length() != 10 && isbn.length() != 13) {
        return false;
    }

    for (int i = 0; i < (int)isbn.length(); i++) {
        if (isbn[i] < '0' || isbn[i] > '9') {
            return false;
        }
    }
    return true;
}

// Validate the details and add one book to the array.
bool addBook(Book books[], int &bookCount, string title, string author,
             string isbn, int copies)
{
    if (bookCount < 0 || bookCount >= MAX_BOOKS) {
        return false;
    }

    title = bookTrimText(title);
    author = bookTrimText(author);
    isbn = bookTrimText(isbn);

    if (!isValidBookText(title) || !isValidBookText(author)) {
        return false;
    }

    if (!isValidIsbn(isbn)) {
        return false;
    }

    if (copies < 1) {
        return false;
    }

    // Reject a book if its title or ISBN is already in the inventory.
    if (findBookByTitle(books, bookCount, title) != -1) {
        return false;
    }
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isbn == isbn) {
            return false;
        }
    }

    books[bookCount].title = title;
    books[bookCount].author = author;
    books[bookCount].isbn = isbn;
    books[bookCount].copiesAvailable = copies;
    books[bookCount].copiesCheckedOut = 0;

    bookCount++;
    return true;
}

// Find a book by title without considering capital or lowercase letters.
// Return its array index, or -1 when no matching title exists.
int findBookByTitle(const Book books[], int bookCount, string title)
{
    title = bookToLowerText(bookTrimText(title));

    for (int i = 0; i < bookCount; i++) {
        if (bookToLowerText(bookTrimText(books[i].title)) == title) {
            return i;
        }
    }
    return -1;
}

// ====================== MERGE END ======================

// Demo helper: safely convert a digit-only string to a positive int.
bool readWholeNumber(string text, int &number)
{
    text = bookTrimText(text);
    if (text.empty()) {
        return false;
    }

    int value = 0;
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }

        int digit = text[i] - '0';
        if (value > (INT_MAX - digit) / 10) {
            return false; // prevent integer overflow
        }
        value = value * 10 + digit;
    }

    if (value < 1) {
        return false;
    }

    number = value;
    return true;
}

// ============================================================
// DEMO main: for Member 2's individual demonstration only.
// Remove this main() when Member 1 merges all members' functions
// into the final group program. Do not merge two main() functions.
// ============================================================
int main()
{
    Book books[MAX_BOOKS];
    int bookCount = 0;
    string choice;

    while (true) {
        cout << "\n===== BOOK MODULE DEMO =====" << endl;
        cout << "1. Add a new book" << endl;
        cout << "2. Search book by title" << endl;
        cout << "3. Display book inventory" << endl;
        cout << "4. Quit" << endl;
        cout << "Enter choice: ";

        if (!getline(cin, choice)) {
            break;
        }

        choice = bookTrimText(choice);

        if (choice == "1") {
            string title, author, isbn, copiesText;
            int copies = 0;

            cout << "Title: ";
            if (!getline(cin, title)) break;
            cout << "Author: ";
            if (!getline(cin, author)) break;
            cout << "ISBN (10 or 13 digits): ";
            if (!getline(cin, isbn)) break;
            cout << "Number of copies: ";
            if (!getline(cin, copiesText)) break;

            if (!readWholeNumber(copiesText, copies)) {
                cout << "Error: copies must be a whole number of 1 or more." << endl;
            }
            else if (!isValidIsbn(isbn)) {
                cout << "Error: ISBN must have exactly 10 or 13 digits." << endl;
            }
            else if (findBookByTitle(books, bookCount, title) != -1) {
                cout << "Error: a book with this title already exists." << endl;
            }
            else if (addBook(books, bookCount, title, author, isbn, copies)) {
                cout << "Book added successfully." << endl;
            }
            else {
                cout << "Book was not added. Check the title and author, "
                     << "the ISBN may already be used, or the book list is full." << endl;
            }
        }
        else if (choice == "2") {
            string title;
            cout << "Enter title to search: ";
            if (!getline(cin, title)) break;

            int index = findBookByTitle(books, bookCount, title);
            if (index == -1) {
                cout << "Book not found." << endl;
            }
            else {
                cout << "Book found:" << endl;
                cout << "Title: " << books[index].title << endl;
                cout << "Author: " << books[index].author << endl;
                cout << "ISBN: " << books[index].isbn << endl;
                cout << "Copies available: " << books[index].copiesAvailable << endl;
                cout << "Copies checked out: " << books[index].copiesCheckedOut << endl;
            }
        }
        else if (choice == "3") {
            if (bookCount == 0) {
                cout << "No books in the inventory yet." << endl;
            }
            else {
                cout << "\n--- BOOK INVENTORY ---" << endl;
                for (int i = 0; i < bookCount; i++) {
                    cout << i + 1 << ". " << books[i].title << endl;
                    cout << "   Author: " << books[i].author << endl;
                    cout << "   ISBN: " << books[i].isbn << endl;
                    cout << "   Copies available: " << books[i].copiesAvailable << endl;
                    cout << "   Copies checked out: " << books[i].copiesCheckedOut << endl;
                }
            }
        }
        else if (choice == "4") {
            cout << "Goodbye." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please enter 1, 2, 3 or 4." << endl;
        }
    }

    return 0;
}

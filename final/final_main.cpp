// final_main.cpp  -  FINAL merged program (Member 1 merges)
// HOW TO MERGE: paste each member's functions into the marked places at the bottom.

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ---------- constants ----------
const int MAX_ADMINS = 10;
const int MAX_BOOKS = 100;
const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;            // most books one borrower can hold

// result codes for borrowBook / returnBook
const int RESULT_OK = 0;
const int ERR_BORROWER_NOT_FOUND = 1;
const int ERR_BOOK_NOT_FOUND = 2;
const int ERR_NO_COPIES = 3;        // borrow only
const int ERR_LOAN_LIMIT = 4;       // borrow only
const int ERR_ALREADY_HAS_BOOK = 5; // borrow only
const int ERR_NOT_BORROWED = 6;     // return only

// ---------- structures ----------
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
    string borrowedTitles[MAX_LOANS];   // titles of books they hold now
    int borrowedCount;
};

// ---------- function prototypes (shared by all members) ----------
// Member 1: login
bool createAdmin(Admin admins[], int &adminCount, string userId, string password);
int  loginAdmin(const Admin admins[], int adminCount, string userId, string password);
// Member 2: books
bool isValidIsbn(string isbn);
bool addBook(Book books[], int &bookCount, string title, string author, string isbn, int copies);
int  findBookByTitle(const Book books[], int bookCount, string title);
// Member 3: borrowers
bool isValidContact(string contact);
bool addBorrower(Borrower borrowers[], int &borrowerCount, string name, string address, string contact);
int  findBorrowerByName(const Borrower borrowers[], int borrowerCount, string name);
// Member 4: borrow and return
int borrowBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle);
int returnBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle);
// Member 5: files
bool saveAdmins(const Admin admins[], int adminCount);
bool loadAdmins(Admin admins[], int &adminCount);
bool saveBooks(const Book books[], int bookCount);
bool loadBooks(Book books[], int &bookCount);
bool saveBorrowers(const Borrower borrowers[], int borrowerCount);
bool loadBorrowers(Borrower borrowers[], int &borrowerCount);

// ---------- Member 1 helper functions (used by main) ----------
int  toMenuChoice(string text);                 // "3" -> 3, anything else -> -1
bool toPositiveInt(string text, int &value);    // whole number of 1 or more
bool isValidText(string text);                  // not empty, no | character
bool isValidUserId(string text);
bool isValidPassword(string text);
bool adminExists(const Admin admins[], int adminCount, string userId);

// =====================================================================
// MAIN of the FINAL program (input and output are done here)
// =====================================================================
int main() {
    // data arrays and counters
    Admin admins[MAX_ADMINS];
    int adminCount = 0;
    Book books[MAX_BOOKS];
    int bookCount = 0;
    Borrower borrowers[MAX_BORROWERS];
    int borrowerCount = 0;

    string line;
    bool running = true;

    // load saved data (on the very first run the files do not exist yet)
    loadAdmins(admins, adminCount);
    loadBooks(books, bookCount);
    loadBorrowers(borrowers, borrowerCount);

    cout << "=====================================\n";
    cout << "       BOOKS BORROWING SYSTEM\n";
    cout << "=====================================\n";

    while (running) {
        // ---------------- START MENU ----------------
        cout << "\n------ START MENU ------\n";
        cout << "1. Create Admin account\n";
        cout << "2. Login\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        if (!getline(cin, line)) {
            line = "3";                          // input closed: quit safely
        }
        int choice = toMenuChoice(line);

        if (choice == 1) {
            // ----- create admin account -----
            string userId, password;
            cout << "New user id: ";
            getline(cin, userId);
            cout << "New password (4 to 20 characters, no spaces): ";
            getline(cin, password);

            if (!isValidUserId(userId)) {
                cout << "Invalid user id. Use 1 to 20 characters, no spaces and no | .\n";
            } else if (!isValidPassword(password)) {
                cout << "Invalid password. Use 4 to 20 characters, no spaces and no | .\n";
            } else if (adminExists(admins, adminCount, userId)) {
                cout << "This user id already exists. Please choose another one.\n";
            } else if (createAdmin(admins, adminCount, userId, password)) {
                saveAdmins(admins, adminCount);
                cout << "Admin account created successfully.\n";
            } else {
                cout << "Cannot create more admin accounts (list is full).\n";
            }

        } else if (choice == 2) {
            // ----- login -----
            string userId, password;
            cout << "User id: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);

            int adminIndex = loginAdmin(admins, adminCount, userId, password);
            if (adminIndex == -1) {
                cout << "Wrong user id or password.\n";
            } else {
                cout << "Login successful. Welcome, " << admins[adminIndex].userId << "!\n";

                // ---------------- ADMIN MENU ----------------
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "\n------ ADMIN MENU ------\n";
                    cout << "1. Add a borrower\n";
                    cout << "2. Add a book\n";
                    cout << "3. Borrow a book\n";
                    cout << "4. Return a book\n";
                    cout << "5. Display book inventory\n";
                    cout << "6. Display all borrowers\n";
                    cout << "7. Logout\n";
                    cout << "Enter your choice: ";
                    if (!getline(cin, line)) {
                        line = "7";              // input closed: logout safely
                    }
                    int menu = toMenuChoice(line);

                    if (menu == 1) {
                        // ----- add borrower -----
                        string name, address, contact;
                        cout << "Borrower name: ";
                        getline(cin, name);
                        cout << "Address: ";
                        getline(cin, address);
                        cout << "Contact number: ";
                        getline(cin, contact);

                        if (!isValidText(name) || !isValidText(address)) {
                            cout << "Name and address must not be empty and must not contain | .\n";
                        } else if (!isValidContact(contact)) {
                            cout << "Invalid contact number (digits, 9 to 15 characters).\n";
                        } else if (findBorrowerByName(borrowers, borrowerCount, name) != -1) {
                            cout << "A borrower with this name already exists.\n";
                        } else if (addBorrower(borrowers, borrowerCount, name, address, contact)) {
                            saveBorrowers(borrowers, borrowerCount);
                            cout << "Borrower added successfully.\n";
                        } else {
                            cout << "Cannot add borrower (list is full).\n";
                        }

                    } else if (menu == 2) {
                        // ----- add book -----
                        string title, author, isbn, copiesText;
                        int copies = 0;
                        cout << "Book title: ";
                        getline(cin, title);
                        cout << "Author: ";
                        getline(cin, author);
                        cout << "ISBN (10 or 13 digits): ";
                        getline(cin, isbn);
                        cout << "Number of copies: ";
                        getline(cin, copiesText);

                        if (!isValidText(title) || !isValidText(author)) {
                            cout << "Title and author must not be empty and must not contain | .\n";
                        } else if (!isValidIsbn(isbn)) {
                            cout << "Invalid ISBN. It must have 10 or 13 digits.\n";
                        } else if (!toPositiveInt(copiesText, copies)) {
                            cout << "Number of copies must be a whole number of 1 or more.\n";
                        } else if (findBookByTitle(books, bookCount, title) != -1) {
                            cout << "A book with this title already exists.\n";
                        } else if (addBook(books, bookCount, title, author, isbn, copies)) {
                            saveBooks(books, bookCount);
                            cout << "Book added successfully.\n";
                        } else {
                            cout << "Cannot add book (ISBN already used or list is full).\n";
                        }

                    } else if (menu == 3 || menu == 4) {
                        // ----- borrow (3) or return (4) -----
                        string borrowerName, bookTitle;
                        cout << "Borrower name: ";
                        getline(cin, borrowerName);
                        cout << "Book title: ";
                        getline(cin, bookTitle);

                        int result;
                        if (menu == 3) {
                            result = borrowBook(borrowers, borrowerCount, books, bookCount, borrowerName, bookTitle);
                        } else {
                            result = returnBook(borrowers, borrowerCount, books, bookCount, borrowerName, bookTitle);
                        }

                        if (result == RESULT_OK) {
                            saveBooks(books, bookCount);
                            saveBorrowers(borrowers, borrowerCount);
                            if (menu == 3) {
                                cout << "Book borrowed successfully. Inventory updated.\n";
                            } else {
                                cout << "Book returned successfully. Inventory updated.\n";
                            }
                        } else if (result == ERR_BORROWER_NOT_FOUND) {
                            cout << "Borrower not found.\n";
                        } else if (result == ERR_BOOK_NOT_FOUND) {
                            cout << "Book not found.\n";
                        } else if (result == ERR_NO_COPIES) {
                            cout << "Sorry, no copies of this book are available now.\n";
                        } else if (result == ERR_LOAN_LIMIT) {
                            cout << "This borrower already holds the maximum number of books (" << MAX_LOANS << ").\n";
                        } else if (result == ERR_ALREADY_HAS_BOOK) {
                            cout << "This borrower already has a copy of this book.\n";
                        } else if (result == ERR_NOT_BORROWED) {
                            cout << "This borrower has not borrowed this book.\n";
                        } else {
                            cout << "Something went wrong. Please try again.\n";
                        }

                    } else if (menu == 5) {
                        // ----- display inventory -----
                        cout << "\n------ BOOK INVENTORY ------\n";
                        if (bookCount == 0) {
                            cout << "No books in the library yet.\n";
                        }
                        for (int i = 0; i < bookCount; i++) {
                            cout << (i + 1) << ". " << books[i].title << "\n";
                            cout << "   Author: " << books[i].author << "\n";
                            cout << "   ISBN: " << books[i].isbn << "\n";
                            cout << "   Available: " << books[i].copiesAvailable
                                 << "   Checked out: " << books[i].copiesCheckedOut << "\n";
                        }

                    } else if (menu == 6) {
                        // ----- display borrowers -----
                        cout << "\n------ ALL BORROWERS ------\n";
                        if (borrowerCount == 0) {
                            cout << "No borrowers yet.\n";
                        }
                        for (int i = 0; i < borrowerCount; i++) {
                            cout << (i + 1) << ". " << borrowers[i].name << "\n";
                            cout << "   Address: " << borrowers[i].address << "\n";
                            cout << "   Contact: " << borrowers[i].contact << "\n";
                            cout << "   Books borrowed: ";
                            if (borrowers[i].borrowedCount == 0) {
                                cout << "none";
                            }
                            for (int j = 0; j < borrowers[i].borrowedCount; j++) {
                                if (j > 0) {
                                    cout << ", ";
                                }
                                cout << borrowers[i].borrowedTitles[j];
                            }
                            cout << "\n";
                        }

                    } else if (menu == 7) {
                        cout << "Logged out.\n";
                        loggedIn = false;

                    } else {
                        cout << "Invalid choice. Please enter a number from 1 to 7.\n";
                    }
                }
            }

        } else if (choice == 3) {
            running = false;

        } else {
            cout << "Invalid choice. Please enter 1, 2 or 3.\n";
        }
    }

    // save everything one last time before the program ends
    saveAdmins(admins, adminCount);
    saveBooks(books, bookCount);
    saveBorrowers(borrowers, borrowerCount);

    cout << "Goodbye!\n";
    return 0;
}

// =====================================================================
// MEMBER 1 FUNCTIONS (login and input checking)
// =====================================================================

// remove spaces at the start and end of a text
string trimText(string text) {
    int start = 0;
    int end = (int)text.length() - 1;
    while (start <= end && text[start] == ' ') {
        start++;
    }
    while (end >= start && text[end] == ' ') {
        end--;
    }
    return text.substr(start, end - start + 1);
}

// Turn a typed menu choice into a number. Returns -1 if it is not a number.
int toMenuChoice(string text) {
    text = trimText(text);
    if (text.empty() || text.length() > 3) {
        return -1;
    }
    int value = 0;
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return -1;
        }
        value = value * 10 + (text[i] - '0');
    }
    return value;
}

// Turn a typed text into a whole number of 1 or more (for number of copies).
bool toPositiveInt(string text, int &value) {
    text = trimText(text);
    if (text.empty() || text.length() > 4) {
        return false;
    }
    int number = 0;
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }
        number = number * 10 + (text[i] - '0');
    }
    if (number < 1) {
        return false;
    }
    value = number;
    return true;
}

// A normal text field (name, address, title, author): not empty, no | character
bool isValidText(string text) {
    text = trimText(text);
    if (text.empty()) {
        return false;
    }
    return text.find('|') == string::npos;
}

// User id: not empty, 20 characters at most, no spaces, no | character
bool isValidUserId(string text) {
    if (text.empty() || text.length() > 20) {
        return false;
    }
    return text.find(' ') == string::npos && text.find('|') == string::npos;
}

// Password: 4 to 20 characters, no spaces, no | character
bool isValidPassword(string text) {
    if (text.length() < 4 || text.length() > 20) {
        return false;
    }
    return text.find(' ') == string::npos && text.find('|') == string::npos;
}

// true if this user id is already used by an admin
bool adminExists(const Admin admins[], int adminCount, string userId) {
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].userId == userId) {
            return true;
        }
    }
    return false;
}

// Create a new admin account. Returns false if the list is full,
// the user id or password is not valid, or the user id already exists.
bool createAdmin(Admin admins[], int &adminCount, string userId, string password) {
    if (adminCount >= MAX_ADMINS) {
        return false;
    }
    if (!isValidUserId(userId) || !isValidPassword(password)) {
        return false;
    }
    if (adminExists(admins, adminCount, userId)) {
        return false;
    }
    admins[adminCount].userId = userId;
    admins[adminCount].password = password;
    adminCount++;
    return true;
}

// Check user id and password. Returns the admin's position (index) in the
// array, or -1 if the user id or password is wrong.
int loginAdmin(const Admin admins[], int adminCount, string userId, string password) {
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].userId == userId && admins[i].password == password) {
            return i;
        }
    }
    return -1;
}

// =====================================================================
// MEMBER 2 FUNCTIONS (books): paste here
//   isValidIsbn, addBook, findBookByTitle
// =====================================================================


// =====================================================================
// MEMBER 3 FUNCTIONS (borrowers): paste here
//   isValidContact, addBorrower, findBorrowerByName
// =====================================================================


// =====================================================================
// MEMBER 4 FUNCTIONS (borrow and return): paste here
//   borrowBook, returnBook
// =====================================================================


// =====================================================================
// MEMBER 5 FUNCTIONS (files): paste here
//   saveAdmins, loadAdmins, saveBooks, loadBooks, saveBorrowers, loadBorrowers
// =====================================================================

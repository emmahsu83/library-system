#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <limits>
using namespace std;

// ============================================================
// BOOKS BORROWING SYSTEM - C++11
// Uses structures and fixed-size arrays (no classes or vectors)
// ============================================================

const int MAX_ADMINS = 10;
const int MAX_BOOKS = 100;
const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;

const int RESULT_OK = 0;
const int ERR_BORROWER_NOT_FOUND = 1;
const int ERR_BOOK_NOT_FOUND = 2;
const int ERR_NO_COPIES = 3;
const int ERR_LOAN_LIMIT = 4;
const int ERR_ALREADY_HAS_BOOK = 5;
const int ERR_NOT_BORROWED = 6;

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

// -------------------- Function prototypes --------------------

// AuthService
bool createAdmin(Admin admins[], int &adminCount,
                 string userId, string password);
int loginAdmin(const Admin admins[], int adminCount,
               string userId, string password);

// AdminService / UserService
bool isValidContact(string contact);
bool addBorrower(Borrower borrowers[], int &borrowerCount,
                 string name, string address, string contact);
int findBorrowerByName(const Borrower borrowers[], int borrowerCount,
                       string name);

// BookService
bool isValidIsbn(string isbn);
bool addBook(Book books[], int &bookCount, string title,
             string author, string isbn, int copies);
int findBookByTitle(const Book books[], int bookCount, string title);
int findBookByName(const Book books[], int bookCount, string name);
bool updateInventory(Book books[], int bookCount, string title, int newCopies);
bool deleteInventory(Book books[], int &bookCount, string title);

// Borrow / Return
int borrowBook(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle);
int returnBook(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle);

// File handling
bool saveAdmins(const Admin admins[], int adminCount);
bool loadAdmins(Admin admins[], int &adminCount);
bool saveBooks(const Book books[], int bookCount);
bool loadBooks(Book books[], int &bookCount);
bool saveBorrowers(const Borrower borrowers[], int borrowerCount);
bool loadBorrowers(Borrower borrowers[], int &borrowerCount);

// Display functions
void displayInventory(const Book books[], int bookCount);
void displayBorrowers(const Borrower borrowers[], int borrowerCount);
void displayAllBooks(const Book books[], int bookCount);
void displayAllUsers(const Borrower borrowers[], int borrowerCount);

// Input helpers
bool containsPipe(string value);
bool isDigits(string value);
string readNonEmpty(string prompt);
int readPositiveInteger(string prompt);
int readMenuChoice(string prompt, int minimum, int maximum);
void saveAll(const Admin admins[], int adminCount,
             const Book books[], int bookCount,
             const Borrower borrowers[], int borrowerCount);
void showBorrowResult(int result);
void showReturnResult(int result);

// Service feature names from the group's Services and Features table
bool CreateUser(Borrower borrowers[], int &borrowerCount,
                string name, string address, string contact);
bool AddBooks(Book books[], int &bookCount, string title,
              string author, string isbn, int copies);
void FindAllUsers(const Borrower borrowers[], int borrowerCount);
int BookBorrow(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle);
int BookReturn(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle);
bool CreateBooks(Book books[], int &bookCount, string title,
                 string author, string isbn, int copies);
void FindAllBooks(const Book books[], int bookCount);
int FindByName(const Book books[], int bookCount, string name);
bool UpdateInventory(Book books[], int bookCount, string title, int newCopies);
bool DeleteInventory(Book books[], int &bookCount, string title);

// ============================================================
// VALIDATION AND SEARCH FUNCTIONS
// ============================================================

bool containsPipe(string value) {
    return value.find('|') != string::npos;
}

bool isDigits(string value) {
    if (value.empty()) return false;

    for (int i = 0; i < (int)value.length(); i++) {
        if (!isdigit(static_cast<unsigned char>(value[i]))) {
            return false;
        }
    }
    return true;
}

bool isValidIsbn(string isbn) {
    if (!isDigits(isbn)) return false;
    return isbn.length() == 10 || isbn.length() == 13;
}

bool isValidContact(string contact) {
    if (contact.length() < 9 || contact.length() > 15) return false;

    int start = 0;
    if (contact[0] == '+' || contact[0] == '-') {
        start = 1;
    }
    if (start == (int)contact.length()) return false;

    for (int i = start; i < (int)contact.length(); i++) {
        if (!isdigit(static_cast<unsigned char>(contact[i]))) {
            return false;
        }
    }
    return true;
}

int findBookByTitle(const Book books[], int bookCount, string title) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == title) return i;
    }
    return -1;
}

int findBookByName(const Book books[], int bookCount, string name) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == name) return i;
    }
    return -1;
}

int findBorrowerByName(const Borrower borrowers[], int borrowerCount,
                       string name) {
    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == name) return i;
    }
    return -1;
}

// ============================================================
// AuthService
// ============================================================

bool createAdmin(Admin admins[], int &adminCount,
                 string userId, string password) {
    if (adminCount >= MAX_ADMINS || userId.empty() ||
        password.length() < 4 || containsPipe(userId) ||
        containsPipe(password)) {
        return false;
    }

    for (int i = 0; i < adminCount; i++) {
        if (admins[i].userId == userId) return false;
    }

    admins[adminCount].userId = userId;
    admins[adminCount].password = password;
    adminCount++;
    return true;
}

int loginAdmin(const Admin admins[], int adminCount,
               string userId, string password) {
    for (int i = 0; i < adminCount; i++) {
        if (admins[i].userId == userId &&
            admins[i].password == password) {
            return i;
        }
    }
    return -1;
}

// ============================================================
// AdminService - CreateUser / AddBooks
// ============================================================

bool addBorrower(Borrower borrowers[], int &borrowerCount,
                 string name, string address, string contact) {
    if (borrowerCount >= MAX_BORROWERS ||
        name.empty() || address.empty() ||
        containsPipe(name) || containsPipe(address) ||
        containsPipe(contact) || !isValidContact(contact)) {
        return false;
    }

    if (findBorrowerByName(borrowers, borrowerCount, name) != -1) {
        return false;
    }

    borrowers[borrowerCount].name = name;
    borrowers[borrowerCount].address = address;
    borrowers[borrowerCount].contact = contact;
    borrowers[borrowerCount].borrowedCount = 0;
    borrowerCount++;
    return true;
}

bool addBook(Book books[], int &bookCount, string title,
             string author, string isbn, int copies) {
    if (bookCount >= MAX_BOOKS || title.empty() || author.empty() ||
        containsPipe(title) || containsPipe(author) || containsPipe(isbn) ||
        !isValidIsbn(isbn) || copies < 1) {
        return false;
    }

    if (findBookByTitle(books, bookCount, title) != -1) {
        return false;
    }

    for (int i = 0; i < bookCount; i++) {
        if (books[i].isbn == isbn) return false;
    }

    books[bookCount].title = title;
    books[bookCount].author = author;
    books[bookCount].isbn = isbn;
    books[bookCount].copiesAvailable = copies;
    books[bookCount].copiesCheckedOut = 0;
    bookCount++;
    return true;
}

// ============================================================
// BookService - Update and Delete Inventory
// ============================================================

bool updateInventory(Book books[], int bookCount,
                     string title, int newCopies) {
    int index = findBookByTitle(books, bookCount, title);

    if (index == -1 || newCopies < 0) return false;

    // Do not allow the total number of copies to be less than
    // the number already borrowed by users.
    books[index].copiesAvailable = newCopies;
    return true;
}

bool deleteInventory(Book books[], int &bookCount, string title) {
    int index = findBookByTitle(books, bookCount, title);

    if (index == -1 || books[index].copiesCheckedOut > 0) {
        return false;
    }

    for (int i = index; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;
    return true;
}

// ============================================================
// UserService - Borrow and AdminService - Return
// ============================================================

int borrowBook(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle) {
    int borrowerIndex = findBorrowerByName(
        borrowers, borrowerCount, borrowerName);
    if (borrowerIndex == -1) return ERR_BORROWER_NOT_FOUND;

    int bookIndex = findBookByTitle(books, bookCount, bookTitle);
    if (bookIndex == -1) return ERR_BOOK_NOT_FOUND;

    if (books[bookIndex].copiesAvailable <= 0) return ERR_NO_COPIES;

    if (borrowers[borrowerIndex].borrowedCount >= MAX_LOANS) {
        return ERR_LOAN_LIMIT;
    }

    for (int i = 0; i < borrowers[borrowerIndex].borrowedCount; i++) {
        if (borrowers[borrowerIndex].borrowedTitles[i] == bookTitle) {
            return ERR_ALREADY_HAS_BOOK;
        }
    }

    borrowers[borrowerIndex].borrowedTitles[
        borrowers[borrowerIndex].borrowedCount] = bookTitle;
    borrowers[borrowerIndex].borrowedCount++;

    books[bookIndex].copiesAvailable--;
    books[bookIndex].copiesCheckedOut++;
    return RESULT_OK;
}

int returnBook(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle) {
    int borrowerIndex = findBorrowerByName(
        borrowers, borrowerCount, borrowerName);
    if (borrowerIndex == -1) return ERR_BORROWER_NOT_FOUND;

    int bookIndex = findBookByTitle(books, bookCount, bookTitle);
    if (bookIndex == -1) return ERR_BOOK_NOT_FOUND;

    int loanIndex = -1;
    for (int i = 0; i < borrowers[borrowerIndex].borrowedCount; i++) {
        if (borrowers[borrowerIndex].borrowedTitles[i] == bookTitle) {
            loanIndex = i;
            break;
        }
    }

    if (loanIndex == -1) return ERR_NOT_BORROWED;

    for (int i = loanIndex;
         i < borrowers[borrowerIndex].borrowedCount - 1; i++) {
        borrowers[borrowerIndex].borrowedTitles[i] =
            borrowers[borrowerIndex].borrowedTitles[i + 1];
    }

    borrowers[borrowerIndex].borrowedCount--;
    borrowers[borrowerIndex].borrowedTitles[
        borrowers[borrowerIndex].borrowedCount] = "";

    books[bookIndex].copiesAvailable++;
    books[bookIndex].copiesCheckedOut--;
    return RESULT_OK;
}

// ============================================================
// File handling - one record per line, fields separated by |
// ============================================================

bool saveAdmins(const Admin admins[], int adminCount) {
    ofstream file("admins.txt");
    if (!file) return false;

    for (int i = 0; i < adminCount; i++) {
        file << admins[i].userId << "|"
             << admins[i].password << "\n";
    }
    return true;
}

bool loadAdmins(Admin admins[], int &adminCount) {
    ifstream file("admins.txt");
    adminCount = 0;
    if (!file) return true; // First run: file may not exist.

    string line;
    while (getline(file, line) && adminCount < MAX_ADMINS) {
        size_t separator = line.find('|');
        if (separator == string::npos) continue;

        string userId = line.substr(0, separator);
        string password = line.substr(separator + 1);

        if (!userId.empty() && password.length() >= 4 &&
            !containsPipe(userId) && !containsPipe(password)) {
            admins[adminCount].userId = userId;
            admins[adminCount].password = password;
            adminCount++;
        }
    }
    return true;
}

bool saveBooks(const Book books[], int bookCount) {
    ofstream file("books.txt");
    if (!file) return false;

    for (int i = 0; i < bookCount; i++) {
        file << books[i].title << "|"
             << books[i].author << "|"
             << books[i].isbn << "|"
             << books[i].copiesAvailable << "|"
             << books[i].copiesCheckedOut << "\n";
    }
    return true;
}

bool loadBooks(Book books[], int &bookCount) {
    ifstream file("books.txt");
    bookCount = 0;
    if (!file) return true;

    string line;
    while (getline(file, line) && bookCount < MAX_BOOKS) {
        string fields[5];
        size_t start = 0;
        bool valid = true;

        for (int i = 0; i < 4; i++) {
            size_t separator = line.find('|', start);
            if (separator == string::npos) {
                valid = false;
                break;
            }
            fields[i] = line.substr(start, separator - start);
            start = separator + 1;
        }
        if (!valid) continue;
        fields[4] = line.substr(start);

        try {
            int available = stoi(fields[3]);
            int checkedOut = stoi(fields[4]);

            if (fields[0].empty() || fields[1].empty() ||
                !isValidIsbn(fields[2]) || available < 0 ||
                checkedOut < 0) continue;

            books[bookCount].title = fields[0];
            books[bookCount].author = fields[1];
            books[bookCount].isbn = fields[2];
            books[bookCount].copiesAvailable = available;
            books[bookCount].copiesCheckedOut = checkedOut;
            bookCount++;
        } catch (...) {
            // Ignore an invalid record instead of crashing.
        }
    }
    return true;
}

bool saveBorrowers(const Borrower borrowers[], int borrowerCount) {
    ofstream file("borrowers.txt");
    if (!file) return false;

    for (int i = 0; i < borrowerCount; i++) {
        file << borrowers[i].name << "|"
             << borrowers[i].address << "|"
             << borrowers[i].contact << "|"
             << borrowers[i].borrowedCount;

        for (int j = 0; j < borrowers[i].borrowedCount; j++) {
            file << "|" << borrowers[i].borrowedTitles[j];
        }
        file << "\n";
    }
    return true;
}

bool loadBorrowers(Borrower borrowers[], int &borrowerCount) {
    ifstream file("borrowers.txt");
    borrowerCount = 0;
    if (!file) return true;

    string line;
    while (getline(file, line) && borrowerCount < MAX_BORROWERS) {
        string fields[4 + MAX_LOANS];
        int fieldCount = 0;
        size_t start = 0;

        while (start <= line.length() &&
               fieldCount < 4 + MAX_LOANS) {
            size_t separator = line.find('|', start);
            if (separator == string::npos) {
                fields[fieldCount++] = line.substr(start);
                break;
            }
            fields[fieldCount++] = line.substr(start, separator - start);
            start = separator + 1;
        }

        if (fieldCount < 4) continue;

        try {
            int count = stoi(fields[3]);
            if (count < 0 || count > MAX_LOANS ||
                fieldCount != 4 + count ||
                fields[0].empty() || fields[1].empty() ||
                !isValidContact(fields[2])) continue;

            borrowers[borrowerCount].name = fields[0];
            borrowers[borrowerCount].address = fields[1];
            borrowers[borrowerCount].contact = fields[2];
            borrowers[borrowerCount].borrowedCount = count;

            for (int j = 0; j < count; j++) {
                borrowers[borrowerCount].borrowedTitles[j] =
                    fields[4 + j];
            }
            borrowerCount++;
        } catch (...) {
            // Ignore an invalid record instead of crashing.
        }
    }
    return true;
}

// ============================================================
// Display functions
// ============================================================

void displayInventory(const Book books[], int bookCount) {
    cout << "\n========== BOOK INVENTORY ==========\n";
    if (bookCount == 0) {
        cout << "No books found.\n";
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        cout << "\nBook " << i + 1 << "\n";
        cout << "Title: " << books[i].title << "\n";
        cout << "Author: " << books[i].author << "\n";
        cout << "ISBN: " << books[i].isbn << "\n";
        cout << "Copies available: "
             << books[i].copiesAvailable << "\n";
        cout << "Copies checked out: "
             << books[i].copiesCheckedOut << "\n";
    }
}

void displayBorrowers(const Borrower borrowers[], int borrowerCount) {
    cout << "\n========== BORROWER LIST ==========\n";
    if (borrowerCount == 0) {
        cout << "No borrowers found.\n";
        return;
    }

    for (int i = 0; i < borrowerCount; i++) {
        cout << "\nBorrower " << i + 1 << "\n";
        cout << "Name: " << borrowers[i].name << "\n";
        cout << "Address: " << borrowers[i].address << "\n";
        cout << "Contact: " << borrowers[i].contact << "\n";
        cout << "Books currently borrowed: ";

        if (borrowers[i].borrowedCount == 0) {
            cout << "None";
        } else {
            for (int j = 0; j < borrowers[i].borrowedCount; j++) {
                cout << borrowers[i].borrowedTitles[j];
                if (j < borrowers[i].borrowedCount - 1) cout << ", ";
            }
        }
        cout << "\n";
    }
}

void displayAllBooks(const Book books[], int bookCount) {
    displayInventory(books, bookCount);
}

void displayAllUsers(const Borrower borrowers[], int borrowerCount) {
    displayBorrowers(borrowers, borrowerCount);
}

// ============================================================
// Service feature names matching the group's table
// ============================================================

bool CreateUser(Borrower borrowers[], int &borrowerCount,
                string name, string address, string contact) {
    return addBorrower(borrowers, borrowerCount, name, address, contact);
}

bool AddBooks(Book books[], int &bookCount, string title,
              string author, string isbn, int copies) {
    return addBook(books, bookCount, title, author, isbn, copies);
}

void FindAllUsers(const Borrower borrowers[], int borrowerCount) {
    displayAllUsers(borrowers, borrowerCount);
}

int BookBorrow(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle) {
    return borrowBook(borrowers, borrowerCount, books, bookCount,
                      borrowerName, bookTitle);
}

int BookReturn(Borrower borrowers[], int borrowerCount,
               Book books[], int bookCount,
               string borrowerName, string bookTitle) {
    return returnBook(borrowers, borrowerCount, books, bookCount,
                      borrowerName, bookTitle);
}

bool CreateBooks(Book books[], int &bookCount, string title,
                 string author, string isbn, int copies) {
    return addBook(books, bookCount, title, author, isbn, copies);
}

void FindAllBooks(const Book books[], int bookCount) {
    displayAllBooks(books, bookCount);
}

int FindByName(const Book books[], int bookCount, string name) {
    return findBookByName(books, bookCount, name);
}

bool UpdateInventory(Book books[], int bookCount,
                     string title, int newCopies) {
    return updateInventory(books, bookCount, title, newCopies);
}

bool DeleteInventory(Book books[], int &bookCount, string title) {
    return deleteInventory(books, bookCount, title);
}

// ============================================================
// Input and message helpers
// ============================================================

string readNonEmpty(string prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);

        if (!value.empty() && !containsPipe(value)) {
            return value;
        }
        cout << "Invalid input. It must not be empty or contain |.\n";
    }
}

int readPositiveInteger(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a whole number greater than zero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readMenuChoice(string prompt, int minimum, int maximum) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= minimum && choice <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cout << "Invalid choice. Enter a number from "
             << minimum << " to " << maximum << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void saveAll(const Admin admins[], int adminCount,
             const Book books[], int bookCount,
             const Borrower borrowers[], int borrowerCount) {
    bool a = saveAdmins(admins, adminCount);
    bool b = saveBooks(books, bookCount);
    bool c = saveBorrowers(borrowers, borrowerCount);

    if (!a || !b || !c) {
        cout << "Warning: one or more data files could not be saved.\n";
    }
}

void showBorrowResult(int result) {
    switch (result) {
        case RESULT_OK:
            cout << "Book borrowed successfully. Inventory updated.\n";
            break;
        case ERR_BORROWER_NOT_FOUND:
            cout << "Borrower not found.\n";
            break;
        case ERR_BOOK_NOT_FOUND:
            cout << "Book not found.\n";
            break;
        case ERR_NO_COPIES:
            cout << "No copies of this book are available.\n";
            break;
        case ERR_LOAN_LIMIT:
            cout << "This borrower has reached the limit of 5 books.\n";
            break;
        case ERR_ALREADY_HAS_BOOK:
            cout << "This borrower already has this book.\n";
            break;
    }
}

void showReturnResult(int result) {
    switch (result) {
        case RESULT_OK:
            cout << "Book returned successfully. Inventory updated.\n";
            break;
        case ERR_BORROWER_NOT_FOUND:
            cout << "Borrower not found.\n";
            break;
        case ERR_BOOK_NOT_FOUND:
            cout << "Book not found.\n";
            break;
        case ERR_NOT_BORROWED:
            cout << "This borrower has not borrowed that book.\n";
            break;
    }
}

// ============================================================
// MAIN PROGRAM
// ============================================================

int main() {
    Admin admins[MAX_ADMINS];
    Book books[MAX_BOOKS];
    Borrower borrowers[MAX_BORROWERS];

    int adminCount = 0;
    int bookCount = 0;
    int borrowerCount = 0;

    // Load saved records when the program starts.
    loadAdmins(admins, adminCount);
    loadBooks(books, bookCount);
    loadBorrowers(borrowers, borrowerCount);

    cout << "====================================\n";
    cout << "       BOOKS BORROWING SYSTEM\n";
    cout << "====================================\n";

    bool programRunning = true;

    while (programRunning) {
        cout << "\n========== START MENU ==========\n";
        cout << "1. Create Admin Account\n";
        cout << "2. Login\n";
        cout << "3. Quit\n";

        int choice = readMenuChoice("Enter your choice: ", 1, 3);

        if (choice == 1) {
            string userId = readNonEmpty("Enter new admin user ID: ");
            string password = readNonEmpty("Enter password (minimum 4 characters): ");

            if (password.length() < 4) {
                cout << "Password must contain at least 4 characters.\n";
            } else if (createAdmin(admins, adminCount, userId, password)) {
                saveAll(admins, adminCount, books, bookCount,
                        borrowers, borrowerCount);
                cout << "Admin account created successfully.\n";
            } else {
                cout << "Could not create account. The ID may already exist "
                     << "or the admin limit may have been reached.\n";
            }
        }
        else if (choice == 2) {
            if (adminCount == 0) {
                cout << "No admin account exists. Create an account first.\n";
                continue;
            }

            string userId = readNonEmpty("Enter admin user ID: ");
            string password = readNonEmpty("Enter password: ");

            int adminIndex = loginAdmin(admins, adminCount, userId, password);

            if (adminIndex == -1) {
                cout << "Incorrect user ID or password.\n";
                continue;
            }

            cout << "Login successful. Welcome, "
                 << admins[adminIndex].userId << "!\n";

            bool loggedIn = true;

            while (loggedIn) {
                cout << "\n========== ADMIN MENU ==========\n";
                cout << "1. Add New Borrower (CreateUser)\n";
                cout << "2. Add New Book (CreateBooks / AddBooks)\n";
                cout << "3. Borrow Book (BookBorrow)\n";
                cout << "4. Return Book (BookReturn)\n";
                cout << "5. Display Book Inventory (FindAllBooks)\n";
                cout << "6. Display All Borrowers (FindAllUsers)\n";
                cout << "7. Find Book by Name (FindByName)\n";
                cout << "8. Update Book Inventory (UpdateInventory)\n";
                cout << "9. Delete Book Inventory (DeleteInventory)\n";
                cout << "10. Logout\n";

                int adminChoice = readMenuChoice("Enter your choice: ", 1, 10);

                if (adminChoice == 1) {
                    string name = readNonEmpty("Enter borrower name: ");
                    string address = readNonEmpty("Enter address: ");

                    string contact;
                    while (true) {
                        contact = readNonEmpty(
                            "Enter contact (9-15 characters; digits, optional leading + or -): ");
                        if (isValidContact(contact)) break;
                        cout << "Invalid contact number. Try again.\n";
                    }

                    if (CreateUser(borrowers, borrowerCount,
                                   name, address, contact)) {
                        saveAll(admins, adminCount, books, bookCount,
                                borrowers, borrowerCount);
                        cout << "Borrower added successfully.\n";
                    } else {
                        cout << "Could not add borrower. Name may already exist "
                             << "or the borrower limit has been reached.\n";
                    }
                }
                else if (adminChoice == 2) {
                    string title = readNonEmpty("Enter book title: ");
                    string author = readNonEmpty("Enter author: ");

                    string isbn;
                    while (true) {
                        isbn = readNonEmpty("Enter ISBN (10 or 13 digits): ");
                        if (isValidIsbn(isbn)) break;
                        cout << "Invalid ISBN. Enter exactly 10 or 13 digits.\n";
                    }

                    int copies = readPositiveInteger("Enter number of copies: ");

                    if (CreateBooks(books, bookCount, title, author,
                                    isbn, copies)) {
                        saveAll(admins, adminCount, books, bookCount,
                                borrowers, borrowerCount);
                        cout << "Book added successfully.\n";
                    } else {
                        cout << "Could not add book. Title or ISBN may already "
                             << "exist, or the book limit has been reached.\n";
                    }
                }
                else if (adminChoice == 3) {
                    string borrowerName = readNonEmpty("Enter borrower name: ");
                    string bookTitle = readNonEmpty("Enter book title: ");

                    int result = BookBorrow(borrowers, borrowerCount,
                                            books, bookCount,
                                            borrowerName, bookTitle);
                    showBorrowResult(result);

                    if (result == RESULT_OK) {
                        saveAll(admins, adminCount, books, bookCount,
                                borrowers, borrowerCount);
                    }
                }
                else if (adminChoice == 4) {
                    string borrowerName = readNonEmpty("Enter borrower name: ");
                    string bookTitle = readNonEmpty("Enter book title: ");

                    int result = BookReturn(borrowers, borrowerCount,
                                            books, bookCount,
                                            borrowerName, bookTitle);
                    showReturnResult(result);

                    if (result == RESULT_OK) {
                        saveAll(admins, adminCount, books, bookCount,
                                borrowers, borrowerCount);
                    }
                }
                else if (adminChoice == 5) {
                    FindAllBooks(books, bookCount);
                }
                else if (adminChoice == 6) {
                    FindAllUsers(borrowers, borrowerCount);
                }
                else if (adminChoice == 7) {
                    string name = readNonEmpty("Enter exact book title to search: ");
                    int index = FindByName(books, bookCount, name);

                    if (index == -1) {
                        cout << "Book not found.\n";
                    } else {
                        cout << "\nBook found:\n";
                        cout << "Title: " << books[index].title << "\n";
                        cout << "Author: " << books[index].author << "\n";
                        cout << "ISBN: " << books[index].isbn << "\n";
                        cout << "Copies available: "
                             << books[index].copiesAvailable << "\n";
                        cout << "Copies checked out: "
                             << books[index].copiesCheckedOut << "\n";
                    }
                }
                else if (adminChoice == 8) {
                    string title = readNonEmpty("Enter exact book title: ");
                    int index = findBookByTitle(books, bookCount, title);

                    if (index == -1) {
                        cout << "Book not found.\n";
                    } else {
                        cout << "Currently available: "
                             << books[index].copiesAvailable << "\n";
                        int newCopies;
                        while (true) {
                            cout << "Enter new number of available copies (0 or more): ";
                            if (cin >> newCopies && newCopies >= 0) {
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                break;
                            }
                            cout << "Enter a whole number of zero or more.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }

                        if (UpdateInventory(books, bookCount, title, newCopies)) {
                            saveAll(admins, adminCount, books, bookCount,
                                    borrowers, borrowerCount);
                            cout << "Inventory updated successfully.\n";
                        } else {
                            cout << "Inventory could not be updated.\n";
                        }
                    }
                }
                else if (adminChoice == 9) {
                    string title = readNonEmpty("Enter exact book title to delete: ");

                    if (DeleteInventory(books, bookCount, title)) {
                        saveAll(admins, adminCount, books, bookCount,
                                borrowers, borrowerCount);
                        cout << "Book deleted successfully.\n";
                    } else {
                        cout << "Book could not be deleted. It may not exist "
                             << "or copies are currently checked out.\n";
                    }
                }
                else if (adminChoice == 10) {
                    saveAll(admins, adminCount, books, bookCount,
                            borrowers, borrowerCount);
                    cout << "You have logged out successfully.\n";
                    loggedIn = false;
                }
            }
        }
        else if (choice == 3) {
            saveAll(admins, adminCount, books, bookCount,
                    borrowers, borrowerCount);
            cout << "Thank you for using the Books Borrowing System.\n";
            programRunning = false;
        }
    }

    return 0;
}

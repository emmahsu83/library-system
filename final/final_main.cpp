// final_main.cpp  -  FINAL merged program (Member 1 merges)
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
// MEMBER 2 FUNCTIONS (books): 
//   isValidIsbn, addBook, findBookByTitle

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

return text.substr(start, end - start + 1);}

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
return true;}

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
return -1;}

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
return true;}
// =====================================================================


// =====================================================================
// MEMBER 3 FUNCTIONS (borrowers): 
//   isValidContact, addBorrower, findBorrowerByName// Remove spaces from beginning and end
string borrowerTrimText(string text)
{
int start = 0;
int end = (int)text.length() - 1;

while (start <= end && text[start] == ' ')
    start++;

while (end >= start && text[end] == ' ')
    end--;

return text.substr(start, end - start + 1);}

// Validate name and address
bool isValidBorrowerText(string text)
{
text = borrowerTrimText(text);

return !text.empty() &&
       text.find('|') == string::npos;}

// Validate contact number
bool isValidContact(string contact)
{
if (contact.length() < 9 ||
contact.length() > 15)
return false;

for (int i = 0; i < (int)contact.length(); i++) {

    char ch = contact[i];

    if (ch >= '0' && ch <= '9')
        continue;

    else if (ch == '-')
        continue;

    else if (ch == '+' && i == 0)
        continue;

    else
        return false;
}

return true;}

// Find borrower by name
int findBorrowerByName(
const Borrower borrowers[],
int borrowerCount,
string name)
{
for (int i = 0; i < borrowerCount; i++) {

    if (borrowers[i].name == name)
        return i;
}

return -1;}

// Add borrower
bool addBorrower(
Borrower borrowers[],
int &borrowerCount,
string name,
string address,
string contact)
{
if (borrowerCount >= MAX_BORROWERS)
return false;

name = borrowerTrimText(name);
address = borrowerTrimText(address);
contact = borrowerTrimText(contact);

if (!isValidBorrowerText(name) ||
    !isValidBorrowerText(address) ||
    !isValidContact(contact))
    return false;

if (findBorrowerByName(
    borrowers, borrowerCount, name) != -1)
    return false;

borrowers[borrowerCount].name = name;
borrowers[borrowerCount].address = address;
borrowers[borrowerCount].contact = contact;

borrowers[borrowerCount].borrowedCount = 0;

borrowerCount++;

return true;}
// =====================================================================


// =====================================================================
// MEMBER 4 FUNCTIONS (borrow and return): 
//   borrowBook, returnBook
// Looks in one borrower's borrowedTitles[] for a title.
// Returns the index (0 .. borrowedCount-1) or -1 if not held.
int findLoanIndex(const Borrower &borrower, string title)
{
for (int i = 0; i < borrower.borrowedCount; i++)
{
if (borrower.borrowedTitles[i] == title)
{
return i;
}
}
return -1;
}

// Lets an existing borrower borrow an available book.
// Returns RESULT_OK on success, otherwise one of the ERR_ codes.
// Nothing is changed unless every check passes.
int borrowBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle)
{
// 1. Borrower must exist
int b = findBorrowerByName(borrowers, borrowerCount, borrowerName);
if (b == -1)
{
return ERR_BORROWER_NOT_FOUND;
}
// 2. Book must exist
int k = findBookByTitle(books, bookCount, bookTitle);
if (k == -1)
{
return ERR_BOOK_NOT_FOUND;
}
// 3. At least one copy must be available
if (books[k].copiesAvailable <= 0)
{
return ERR_NO_COPIES;
}
// 4. Borrower must be under the loan limit
if (borrowers[b].borrowedCount >= MAX_LOANS)
{
return ERR_LOAN_LIMIT;
}
// 5. Borrower must not already hold this title
if (findLoanIndex(borrowers[b], books[k].title) != -1)
{
return ERR_ALREADY_HAS_BOOK;
}
// All checks passed: update inventory and borrower record
books[k].copiesAvailable--;
books[k].copiesCheckedOut++;
borrowers[b].borrowedTitles[borrowers[b].borrowedCount] = books[k].title;
borrowers[b].borrowedCount++;

return RESULT_OK;}

// Lets a borrower return a book they currently hold.
// Returns RESULT_OK on success, otherwise one of the ERR_ codes
// (it can also return ERR_NOT_BORROWED).
int returnBook(Borrower borrowers[], int borrowerCount, Book books[], int bookCount, string borrowerName, string bookTitle)
{
// 1. Borrower must exist
int b = findBorrowerByName(borrowers, borrowerCount, borrowerName);
if (b == -1)
{
return ERR_BORROWER_NOT_FOUND;
}
// 2. Book must exist
int k = findBookByTitle(books, bookCount, bookTitle);
if (k == -1)
{
return ERR_BOOK_NOT_FOUND;
}
// 3. Borrower must actually hold this title
int loanIndex = findLoanIndex(borrowers[b], books[k].title);
if (loanIndex == -1)
{
return ERR_NOT_BORROWED;
}

// Remove the title: shift everything after it one place to the left
for (int i = loanIndex; i < borrowers[b].borrowedCount - 1; i++)
{
    borrowers[b].borrowedTitles[i] = borrowers[b].borrowedTitles[i + 1];
}
// Clear the last used slot and reduce the counter
borrowers[b].borrowedTitles[borrowers[b].borrowedCount - 1] = "";
borrowers[b].borrowedCount--;

// Update the inventory
books[k].copiesAvailable++;
books[k].copiesCheckedOut--;

return RESULT_OK;}
// =====================================================================


// =====================================================================
// MEMBER 5 FUNCTIONS (files): 
//   saveAdmins, loadAdmins, saveBooks, loadBooks, saveBorrowers, loadBorrowers
for (int j = 0; j < borrowers[i].borrowedCount; j++) {

        if (borrowers[i].borrowedTitles[j].find('|') != string::npos) {
            outFile.close();
            return false;
        }

        outFile << "|" << borrowers[i].borrowedTitles[j];
    }

    outFile << endl;
}

outFile.close();
return true;}

// 6. Load Borrower Information
bool loadBorrowers(Borrower borrowers[], int &borrowerCount) {
ifstream inFile("borrowers.txt");
borrowerCount = 0;

if (!inFile)
    return true;

string line;

while (getline(inFile, line)) {
    if (borrowerCount >= MAX_BORROWERS) {
        inFile.close();
        return false;
    }

    stringstream ss(line);
    string name, address, contact, countText;

    if (!getline(ss, name, '|') ||
        !getline(ss, address, '|') ||
        !getline(ss, contact, '|') ||
        !getline(ss, countText, '|')) {
        inFile.close();
        return false;
    }

    int count;

    try {
        size_t pos;
        count = stoi(countText, &pos);

        if (pos != countText.size() ||
            count < 0 || count > MAX_LOANS) {
            inFile.close();
            return false;
        }
    }
    catch (...) {
        inFile.close();
        return false;
    }

    borrowers[borrowerCount].name = name;
    borrowers[borrowerCount].address = address;
    borrowers[borrowerCount].contact = contact;
    borrowers[borrowerCount].borrowedCount = count;

    for (int j = 0; j < count; j++) {
        string title;

        if (!getline(ss, title, '|')) {
            inFile.close();
            return false;
        }

        borrowers[borrowerCount].borrowedTitles[j] = title;
    }

    // Check for extra fields
    string extra;
    if (getline(ss, extra, '|')) {
        inFile.close();
        return false;
    }

    borrowerCount++;
}

inFile.close();
return true;}
// =====================================================================

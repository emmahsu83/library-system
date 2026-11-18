// ============================================================
// File      : Member5_Files.cpp
// Module    : Files (Member 5)
// Course    : PRG2203 Programming Techniques - Coursework 3
// Author    : [PAN THITSAR OO AND I26042516]
// Date      : [13/11/2026]
// Compile   : g++ -std=c++11 Member5_Files.cpp -o files
// Functions : Save and load functions for admins, books, and borrowers
// ============================================================
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


const int MAX_ADMINS = 10;
const int MAX_BOOKS = 100;
const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;


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

// ========================================
// MEMBER 5 FUNCTIONS (FILES)
// ========================================

// 1. Save Admin Information
bool saveAdmins(const Admin admins[], int adminCount) {
if (adminCount < 0 || adminCount > MAX_ADMINS)
return false;

ofstream outFile("admins.txt");

if (!outFile)
    return false;

for (int i = 0; i < adminCount; i++) {
    if (admins[i].userId.find('|') != string::npos ||
        admins[i].password.find('|') != string::npos) {
        outFile.close();
        return false;
    }

    outFile << admins[i].userId << "|"
            << admins[i].password << endl;
}

outFile.close();
return true;
}

// 2. Load Admin Information
bool loadAdmins(Admin admins[], int &adminCount) {
ifstream inFile("admins.txt");
adminCount = 0;

if (!inFile)
    return true;

string line;

while (getline(inFile, line)) {
    if (adminCount >= MAX_ADMINS) {
        inFile.close();
        return false;
    }

    stringstream ss(line);
    string userId, password;

    if (!getline(ss, userId, '|') ||
        !getline(ss, password) ||
        password.find('|') != string::npos ||
        userId.empty()) {
        inFile.close();
        return false;
    }

    admins[adminCount].userId = userId;
    admins[adminCount].password = password;

    adminCount++;
}

inFile.close();
return true;
}

// ========================================
// 3. Save Book Information
// ========================================
bool saveBooks(const Book books[], int bookCount) {
if (bookCount < 0 || bookCount > MAX_BOOKS)
return false;

ofstream outFile("books.txt");

if (!outFile)
    return false;

for (int i = 0; i < bookCount; i++) {
    if (books[i].title.find('|') != string::npos ||
        books[i].author.find('|') != string::npos ||
        books[i].isbn.find('|') != string::npos) {
        outFile.close();
        return false;
    }

    outFile << books[i].title << "|"
            << books[i].author << "|"
            << books[i].isbn << "|"
            << books[i].copiesAvailable << "|"
            << books[i].copiesCheckedOut << endl;
}

outFile.close();
return true;
}

// ========================================
// 4. Load Book Information
// ========================================
bool loadBooks(Book books[], int &bookCount) {
ifstream inFile("books.txt");
bookCount = 0;

if (!inFile)
    return true;

string line;

while (getline(inFile, line)) {
    if (bookCount >= MAX_BOOKS) {
        inFile.close();
        return false;
    }

    stringstream ss(line);
    string title, author, isbn;
    string availableText, checkedOutText;

    if (!getline(ss, title, '|') ||
        !getline(ss, author, '|') ||
        !getline(ss, isbn, '|') ||
        !getline(ss, availableText, '|') ||
        !getline(ss, checkedOutText) ||
        checkedOutText.find('|') != string::npos) {
        inFile.close();
        return false;
    }

    int available, checkedOut;

    try {
        size_t p1, p2;
        available = stoi(availableText, &p1);
        checkedOut = stoi(checkedOutText, &p2);

        if (p1 != availableText.size() ||
            p2 != checkedOutText.size() ||
            available < 0 || checkedOut < 0) {
            inFile.close();
            return false;
        }
    }
    catch (...) {
        inFile.close();
        return false;
    }

    books[bookCount].title = title;
    books[bookCount].author = author;
    books[bookCount].isbn = isbn;
    books[bookCount].copiesAvailable = available;
    books[bookCount].copiesCheckedOut = checkedOut;

    bookCount++;
}

inFile.
close();
return true;
}

// ========================================
// 5. Save Borrower Information
// ========================================
bool saveBorrowers(const Borrower borrowers[],
int borrowerCount) {
if (borrowerCount < 0 ||
borrowerCount > MAX_BORROWERS)
return false;

ofstream outFile("borrowers.txt");

if (!outFile)
    return false;

for (int i = 0; i < borrowerCount; i++) {
    if (borrowers[i].borrowedCount < 0 ||
        borrowers[i].borrowedCount > MAX_LOANS ||
        borrowers[i].name.find('|') != string::npos ||
        borrowers[i].address.find('|') != string::npos ||
        borrowers[i].contact.find('|') != string::npos) {
        outFile.close();
        return false;
    }

    outFile << borrowers[i].name << "|"
            << borrowers[i].address << "|"
            << borrowers[i].contact << "|"
            << borrowers[i].borrowedCount;

    for (int j = 0;
         j < borrowers[i].borrowedCount; j++) {

        if (borrowers[i].borrowedTitles[j].find('|')
            != string::npos) {
            outFile.close();
            return false;
        }

        outFile << "|"
                << borrowers[i].borrowedTitles[j];
    }

    outFile << endl;
}

outFile.close();
return true;
}

// ========================================
// 6. Load Borrower Information
// ========================================
bool loadBorrowers(Borrower borrowers[],
int &borrowerCount) {
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
return true;
}

// ========================================
// INDIVIDUAL TEST MAIN
// ========================================
int main() {
Admin admins[MAX_ADMINS];
Book books[MAX_BOOKS];
Borrower borrowers[MAX_BORROWERS];

int adminCount = 1;
int bookCount = 1;
int borrowerCount = 1;

// Sample Admin
admins[0].userId = "admin01";
admins[0].password = "12345";

// Sample Book
books[0].title = "Harry Potter";
books[0].author = "J.K. Rowling";
books[0].isbn = "1234567890";
books[0].copiesAvailable = 5;
books[0].copiesCheckedOut = 0;

// Sample Borrower
borrowers[0].name = "John";
borrowers[0].address = "Kuala Lumpur";
borrowers[0].contact = "0123456789";
borrowers[0].borrowedCount = 1;
borrowers[0].borrowedTitles[0] = "Harry Potter";

// Save all information
bool saved = saveAdmins(admins, adminCount) &&
             saveBooks(books, bookCount) &&
             saveBorrowers(borrowers, borrowerCount);

cout << "Saving data: "
     << (saved ? "Successful" : "Failed") << endl;

// Reset counters before loading
adminCount = 0;
bookCount = 0;
borrowerCount = 0;

// Load all information
bool loaded = loadAdmins(admins, adminCount) &&
              loadBooks(books, bookCount) &&
              loadBorrowers(borrowers, borrowerCount);
              
              cout << "Loading data: "
     << (loaded ? "Successful" : "Failed") << endl;

if (loaded) {
    cout << "\nAdmin count: " << adminCount;
    cout << "\nBook count: " << bookCount;
    cout << "\nBorrower count: " << borrowerCount;

    cout << "\n\nBook title: " << books[0].title;
    cout << "\nBorrower name: " << borrowers[0].name;
}

return 0;
}
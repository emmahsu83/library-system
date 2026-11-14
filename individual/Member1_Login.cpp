// Member1_Login.cpp  -  Member 1 individual program
// Demonstrates: functions, structure, array, control structures, input validation

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
// MAIN for Member 1's own demo program (create admin, login, quit)
// =====================================================================
int main() {
    Admin admins[MAX_ADMINS];
    int adminCount = 0;
    string line;
    bool running = true;

    cout << "=== Member 1 demo: Create Admin account and Login ===\n";

    while (running) {
        cout << "\n------ START MENU ------\n";
        cout << "1. Create Admin account\n";
        cout << "2. Login\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        if (!getline(cin, line)) {
            line = "3";                      // input closed: quit safely
        }
        int choice = toMenuChoice(line);

        if (choice == 1) {
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
                cout << "Admin account created successfully.\n";
            } else {
                cout << "Cannot create more admin accounts (list is full).\n";
            }
        } else if (choice == 2) {
            string userId, password;
            cout << "User id: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);

            int index = loginAdmin(admins, adminCount, userId, password);
            if (index == -1) {
                cout << "Wrong user id or password.\n";
            } else {
                cout << "Login successful. Welcome, " << admins[index].userId << "!\n";
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "\n--- After login (demo) ---\n";
                    cout << "1. Logout\n";
                    cout << "Enter your choice: ";
                    if (!getline(cin, line)) {
                        line = "1";
                    }
                    if (toMenuChoice(line) == 1) {
                        cout << "Logged out.\n";
                        loggedIn = false;
                    } else {
                        cout << "Invalid choice. Please enter 1.\n";
                    }
                }
            }
        } else if (choice == 3) {
            running = false;
        } else {
            cout << "Invalid choice. Please enter 1, 2 or 3.\n";
        }
    }

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

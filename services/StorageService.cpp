#include "StorageService.h"
#include "../models/Book.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Helper function: Save a book
static void writeBook(ofstream& file, const Book& book) {
file << book.title << '\n';
file << book.author << '\n';
file << book.isbn << '\n';
file << book.nocAvailable << '\n';
file << book.nocCheckOut << '\n';
}

// Helper function: Load a book
static bool readBook(ifstream& file, Book& book) {
string line;

if (!getline(file, book.title)) return false;
if (!getline(file, book.author)) return false;
if (!getline(file, book.isbn)) return false;

if (!getline(file, line)) return false;
stringstream(line) >> book.nocAvailable;

if (!getline(file, line)) return false;
stringstream(line) >> book.nocCheckOut;

return true;
}

// Helper function: Read a number
static bool readNumber(ifstream& file, int& number) {
string line;

if (!getline(file, line)) return false;

stringstream ss(line);
if (!(ss >> number) || number < 0) return false;

return true;
}

// 1. Save Books
bool StorageService::saveBooks(const vector<Book>& books) {
ofstream file("books.txt");

if (!file.is_open()) return false;

file << books.size() << '\n';

for (const Book& book : books) {
    writeBook(file, book);
}

return file.good();
}

// 2. Load Books
bool StorageService::loadBooks(vector<Book>& books) {
ifstream file("books.txt");

if (!file.is_open()) return false;

int count;
if (!readNumber(file, count)) return false;

vector<Book> loadedBooks;

for (int i = 0; i < count; i++) {
    Book book;

    if (!readBook(file, book)) return false;

    loadedBooks.push_back(book);
}

books = loadedBooks;
return true;
}

// 3. Save Users
bool StorageService::saveUsers(const vector<User>& users) {
ofstream file("users.txt");

if (!file.is_open()) return false;

file << users.size() << '\n';

for (const User& user : users) {
    file << user.name << '\n';
    file << user.address << '\n';
    file << user.contact << '\n';
    file << user.password << '\n';

    file << user.books.size() << '\n';

    for (const Book& book : user.books) {
        writeBook(file, book);
    }
}

return file.good();
}

// 4. Load Users
bool StorageService::loadUsers(vector<User>& users) {
ifstream file("users.txt");

if (!file.is_open()) return false;

int count;
if (!readNumber(file, count)) return false;

vector<User> loadedUsers;

for (int i = 0; i < count; i++) {
    User user;
    int bookCount;

    if (!getline(file, user.name)) return false;
    if (!getline(file, user.address)) return false;
    if (!getline(file, user.contact)) return false;
    if (!getline(file, user.password)) return false;

    if (!readNumber(file, bookCount)) return false;

    for (int j = 0; j < bookCount; j++) {
        Book book;

        if (!readBook(file, book)) return false;

        user.addBook(book);
    }

    loadedUsers.push_back(user);
}

users = loadedUsers;
return true;
}

// 5. Save Admins
bool StorageService::saveAdmins(const vector<Admin>& admins) {
ofstream file("admins.txt");

if (!file.is_open()) return false;

file << admins.size() << '\n';

for (const Admin& admin : admins) {
    file << admin.name << '\n';
    file << admin.address << '\n';
    file << admin.contact << '\n';
    file << admin.password << '\n';
}

return file.good();
}

// 6. Load Admins
bool StorageService::loadAdmins(vector<Admin>& admins) {
ifstream file("admins.txt");

if (!file.is_open()) return false;

int count;
if (!readNumber(file, count)) return false;

vector<Admin> loadedAdmins;

for (int i = 0; i < count; i++) {
    Admin admin;

    if (!getline(file, admin.name)) return false;
    if (!getline(file, admin.address)) return false;
    if (!getline(file, admin.contact)) return false;
    if (!getline(file, admin.password)) return false;

    loadedAdmins.push_back(admin);
}

admins = loadedAdmins;
return true;
}
// ============================================================
// File    : Member3_Borrower.cpp
// Module  : Borrower (Member 3)
// Course  : PRG2203 Programming Techniques - Coursework 3
// ============================================================

#include <iostream>
#include <string>
using namespace std;

const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;

struct Borrower {
    string name;
    string address;
    string contact;
    string borrowedTitles[MAX_LOANS];
    int borrowedCount;
};

// Function prototypes
string borrowerTrimText(string text);
bool isValidBorrowerText(string text);
bool isValidContact(string contact);

int findBorrowerByName(
    const Borrower borrowers[],
    int borrowerCount,
    string name
);

bool addBorrower(
    Borrower borrowers[],
    int &borrowerCount,
    string name,
    string address,
    string contact
);

// Remove spaces from beginning and end
string borrowerTrimText(string text)
{
    int start = 0;
    int end = (int)text.length() - 1;

    while (start <= end && text[start] == ' ')
        start++;

    while (end >= start && text[end] == ' ')
        end--;

    return text.substr(start, end - start + 1);
}

// Validate name and address
bool isValidBorrowerText(string text)
{
    text = borrowerTrimText(text);

    return !text.empty() &&
           text.find('|') == string::npos;
}

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

    return true;
}

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

    return -1;
}

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

    return true;
}

// ============================================================
// MAIN PROGRAM
// ============================================================

int main()
{
    Borrower borrowers[MAX_BORROWERS];
    int borrowerCount = 0;

    string choice;

    while (true) {

        cout << "\n===== BORROWER DEMO =====" << endl;
        cout << "1. Add Borrower" << endl;
        cout << "2. Display All Borrowers" << endl;
        cout << "3. Find Borrower by Name" << endl;
        cout << "4. Exit" << endl;

        cout << "\nEnter your choice: ";

        if (!getline(cin, choice))
            break;

        choice = borrowerTrimText(choice);

        // OPTION 1: ADD BORROWER
        if (choice == "1") {

            string name, address, contact;

            cout << "\n===== ADD BORROWER =====" << endl;

            cout << "Enter borrower name: ";
            getline(cin, name);

            cout << "Enter address: ";
            getline(cin, address);

            cout << "Enter contact number: ";
            getline(cin, contact);

            name = borrowerTrimText(name);
            address = borrowerTrimText(address);
            contact = borrowerTrimText(contact);

            if (!isValidBorrowerText(name)) {
                cout << "Invalid borrower name." << endl;
            }

            else if (!isValidBorrowerText(address)) {
                cout << "Invalid borrower address." << endl;
            }

            else if (!isValidContact(contact)) {
                cout << "Invalid contact number." << endl;
            }

            else if (findBorrowerByName(
                borrowers, borrowerCount, name) != -1) {

                cout << "Borrower already exists." << endl;
            }

            else if (addBorrower(
                borrowers, borrowerCount,
                name, address, contact)) {

                cout << "\nBorrower added successfully!" << endl;
            }

            else {
                cout << "Cannot add borrower. Borrower list is full."
                     << endl;
            }
        }

        // OPTION 2: DISPLAY ALL BORROWERS
        else if (choice == "2") {

            cout << "\n===== BORROWER LIST =====" << endl;

            if (borrowerCount == 0) {

                cout << "No borrowers found." << endl;
            }

            else {

                for (int i = 0; i < borrowerCount; i++) {

                    cout << "\nBorrower " << i + 1 << endl;

                    cout << "Name: "
                         << borrowers[i].name << endl;

                    cout << "Address: "
                         << borrowers[i].address << endl;

                    cout << "Contact: "
                         << borrowers[i].contact << endl;

                    cout << "Books currently borrowed: ";

                    if (borrowers[i].borrowedCount == 0) {
                        cout << "None" << endl;
                    }

                    else {

                        for (int j = 0;
                             j < borrowers[i].borrowedCount; j++) {

                            cout << borrowers[i].borrowedTitles[j];

                            if (j < borrowers[i].borrowedCount - 1)
                                cout << ", ";
                        }

                        cout << endl;
                    }
                }
            }
        }

        // OPTION 3: FIND BORROWER BY NAME
        else if (choice == "3") {

            string searchName;

            cout << "\nEnter borrower name to search: ";
            getline(cin, searchName);

            searchName = borrowerTrimText(searchName);

            int index = findBorrowerByName(
                borrowers, borrowerCount, searchName);

            if (index == -1) {

                cout << "\nBorrower not found." << endl;
            }

            else {

                cout << "\nBorrower found!" << endl;

                cout << "\nName: "
                     << borrowers[index].name << endl;

                cout << "Address: "
                     << borrowers[index].address << endl;

                cout << "Contact: "
                     << borrowers[index].contact << endl;

                cout << "Books currently borrowed: ";

                if (borrowers[index].borrowedCount == 0) {
                    cout << "None" << endl;
                }

                else {

                    for (int j = 0;
                         j < borrowers[index].borrowedCount; j++) {

                        cout << borrowers[index].borrowedTitles[j];

                        if (j < borrowers[index].borrowedCount - 1)
                            cout << ", ";
                    }

                    cout << endl;
                }
            }
        }

        // OPTION 4: EXIT
        else if (choice == "4") {

            cout << "\nExiting Borrower Demo..." << endl;
            break;
        }

        else {

            cout << "Invalid choice. Please enter 1, 2, 3 or 4."
                 << endl;
        }
    }

    return 0;
}
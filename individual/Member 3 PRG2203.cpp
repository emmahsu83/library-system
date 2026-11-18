
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
using namespace std;

const int MAX_BORROWERS = 100;
const int MAX_LOANS = 5;

// Borrower structure
struct Borrower {
    string name;
    string address;
    string contact;
    string borrowedTitles[MAX_LOANS];
    int borrowedCount;
};

// 1. Validate contact number
bool isValidContact(string contact) {
    if (contact.length() < 9 ||
        contact.length() > 15)
        return false;

    int start = 0;

    if (contact[0] == '+' || contact[0] == '-')
        start = 1;

    if (start == (int)contact.length())
        return false;

    for (int i = start;
         i < (int)contact.length(); i++) {

        if (!isdigit(
            static_cast<unsigned char>(contact[i])))
            return false;
    }

    return true;
}

// 2. Find borrower by name
int findBorrowerByName(
    const Borrower borrowers[],
    int borrowerCount,
    string name) {

    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == name)
            return i;
    }

    return -1;
}

// 3. Add a new borrower
bool addBorrower(
    Borrower borrowers[],
    int &borrowerCount,
    string name,
    string address,
    string contact) {

    if (borrowerCount >= MAX_BORROWERS ||
        name.empty() || address.empty() ||
        name.find('|') != string::npos ||
        address.find('|') != string::npos ||
        contact.find('|') != string::npos ||
        !isValidContact(contact)) {
        return false;
    }

    if (findBorrowerByName(
        borrowers, borrowerCount, name) != -1) {
        return false;
    }

    borrowers[borrowerCount].name = name;
    borrowers[borrowerCount].address = address;
    borrowers[borrowerCount].contact = contact;
    borrowers[borrowerCount].borrowedCount = 0;

    borrowerCount++;

    return true;
}

// 4. Display all borrowers
void displayBorrowers(
    const Borrower borrowers[],
    int borrowerCount) {

    cout << "\n========== BORROWER LIST ==========\n";

    if (borrowerCount == 0) {
        cout << "No borrowers found.\n";
        return;
    }

    for (int i = 0; i < borrowerCount; i++) {
        cout << "\nBorrower " << i + 1 << endl;
        cout << "Name: " << borrowers[i].name << endl;
        cout << "Address: " << borrowers[i].address << endl;
        cout << "Contact: " << borrowers[i].contact << endl;

        cout << "Books currently borrowed: ";

        if (borrowers[i].borrowedCount == 0) {
            cout << "None";
        }
        else {
            for (int j = 0;
                 j < borrowers[i].borrowedCount; j++) {

                cout << borrowers[i].borrowedTitles[j];

                if (j < borrowers[i].borrowedCount - 1)
                    cout << ", ";
            }
        }

        cout << endl;
    }
}

// Main demo program
int main() {
    Borrower borrowers[MAX_BORROWERS];
    int borrowerCount = 0;
    int choice;

    do {
        cout << "\n===== BORROWER DEMO =====\n";
        cout << "1. Add Borrower\n";
        cout << "2. Display All Borrowers\n";
        cout << "3. Find Borrower by Name\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );
            cout << "Invalid choice.\n";
            continue;
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        switch (choice) {
            case 1: {
                string name, address, contact;

                cout << "Enter borrower name: ";
                getline(cin, name);

                cout << "Enter address: ";
                getline(cin, address);

                cout << "Enter contact: ";
                getline(cin, contact);

                if (addBorrower(
                    borrowers, borrowerCount,
                    name, address, contact)) {

                    cout << "Borrower added successfully.\n";
                }
                else {
                    cout << "Could not add borrower.\n";
                    cout << "Check contact, duplicate name "
                         << "or borrower limit.\n";
                }

                break;
            }

            case 2:
                displayBorrowers(
                    borrowers, borrowerCount
                );
                break;

            case 3: {
                string name;

                cout << "Enter exact borrower name: ";
                getline(cin, name);

                int index = findBorrowerByName(
                    borrowers, borrowerCount, name
                );

                if (index == -1) {
                    cout << "Borrower not found.\n";
                }
                else {
                    cout << "\nBorrower found!\n";
                    cout << "Name: "
                         << borrowers[index].name << endl;
                    cout << "Address: "
                         << borrowers[index].address << endl;
                    cout << "Contact: "
                         << borrowers[index].contact << endl;
                }

                break;
            }

            case 4:
                cout << "Demo ended.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 4);

    return 0;
}

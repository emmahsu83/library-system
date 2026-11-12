// Owner: Member 1 (Project lead / Auth)
// The welcome screen. show() returns 1 for Login, 0 for Exit.
#include "HomePage.h"
#include <iostream>
#include <limits>

int HomePage::show() {
    while (true) {
        std::cout << "\n===============================\n";
        std::cout << "      LIBRARY MANAGEMENT SYSTEM\n";
        std::cout << "===============================\n";
        std::cout << "1. Login\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                return 0;                       // input closed: exit safely
            }
            std::cin.clear();                   // not a number: clear the error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }

        // Remove the rest of the line, so later std::getline calls work
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1 || choice == 0) {
            return choice;
        }
        std::cout << "Invalid choice. Please enter 1 or 0.\n";
    }
}

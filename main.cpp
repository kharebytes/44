#include "Library.h"

void showMenu() {
    cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Search Book (with location)" << endl;
    cout << "0. Exit" << endl;
}

int main() {
    Library lib;

    while (true) {
        showMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: lib.addBookFromInput(); break;
            case 2: lib.searchBook(); break;
            case 0: cout << "Goodbye!" << endl; return 0;
            default: cout << "Invalid choice. Try again." << endl;
        }
    }
}

// Owner: Shushant (menu and integration).
#include "Library.h"
#include "MemberOps.h"
#include "Circulation.h"
#include "Storage.h"

void showMenu() {
    cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Search Book (with location)" << endl;
    cout << "3. Register Member" << endl;
    cout << "4. View Members" << endl;
    cout << "5. Issue Book" << endl;
    cout << "6. Return Book (guided shelving)" << endl;
    cout << "0. Save data and Exit" << endl;
}

int main() {
    Library lib;
    lib.loadFromFile();

    while (true) {
        showMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: lib.addBookFromInput();        lib.saveToFile(); break;
            case 2: lib.searchBook();                                break;
            case 3: lib.registerMemberFromInput(); lib.saveToFile(); break;
            case 4: lib.viewMembers();                               break;
            case 5: lib.issueBook();               lib.saveToFile(); break;
            case 6: lib.returnBook();              lib.saveToFile(); break;
            case 0: lib.saveToFile(); cout << "Goodbye!" << endl; return 0;
            default: cout << "Invalid choice. Try again." << endl;
        }
    }
}

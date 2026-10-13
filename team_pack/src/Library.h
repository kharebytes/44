#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include "Book.h"
#include "Member.h"
#include "Input.h"
using namespace std;

// The Library class is declared here (owner: Utpal). Other team members
// DEFINE their functions in their own header files using
//     inline ReturnType Library::functionName(...) { ... }
// so nobody needs to edit this file.
class Library {
private:
    vector<Book> books;
    map<int, Member> members;

public:
    static const int ALLOWED_DAYS = 14;   // days a book can be kept
    static const int FINE_PER_DAY = 2;    // Rs. per extra day

    // ---------- Books (owner: Utpal) ----------
    int findIndexById(int id) const {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == id) return (int)i;
        }
        return -1;
    }

    // Pointer to the book with this ID, or nullptr if it does not exist.
    Book* getBookById(int id) {
        int i = findIndexById(id);
        return (i == -1) ? nullptr : &books[i];
    }

    const vector<Book>& getBooks() const { return books; }

    // Returns false if a book with the same ID already exists.
    bool addBook(const Book& b) {
        if (findIndexById(b.getId()) != -1) return false;
        books.push_back(b);
        return true;
    }

    void addBookFromInput() {
        int id = readInt("Book ID: ");
        if (findIndexById(id) != -1) {
            cout << "A book with this ID already exists." << endl;
            return;
        }
        string title = cleanText(readLine("Title: "));
        string author = cleanText(readLine("Author: "));
        int qty = readInt("Quantity: ");
        if (qty < 0) {
            cout << "Quantity cannot be negative." << endl;
            return;
        }
        string section = cleanText(readLine("Section (e.g. CSE): "));
        int rack = readInt("Rack number: ");
        int shelf = readInt("Shelf number: ");

        addBook(Book(id, title, author, qty, section, rack, shelf));
        cout << "Book added successfully." << endl;
    }

    void searchBook() const {
        string key = readLine("Search by ID, title or author: ");
        bool isNumber = !key.empty() && key.size() <= 9 &&
                        all_of(key.begin(), key.end(), ::isdigit);
        int idKey = isNumber ? stoi(key) : -1;

        int count = 0;
        for (size_t i = 0; i < books.size(); i++) {
            const Book& b = books[i];
            if (b.matches(key) || (isNumber && b.getId() == idKey)) {
                cout << "------------------------------" << endl;
                b.display();
                count++;
            }
        }
        if (count == 0) cout << "No book found." << endl;
        else cout << "------------------------------\n" << count << " book(s) found." << endl;
    }

    // ---------- Members (accessors by Utpal, input/view by Udit) ----------
    Member* getMemberById(int id) {
        map<int, Member>::iterator it = members.find(id);
        return (it == members.end()) ? nullptr : &(it->second);
    }

    const map<int, Member>& getMembers() const { return members; }

    // Returns false if a member with the same ID already exists.
    bool addMember(const Member& m) {
        if (members.find(m.getId()) != members.end()) return false;
        members[m.getId()] = m;
        return true;
    }

    void registerMemberFromInput();   // defined in MemberOps.h   (Udit)
    void viewMembers() const;         // defined in MemberOps.h   (Udit)

    // ---------- Circulation (owner: Srajen) ----------
    void issueBook();                                              // Circulation.h
    void returnBook();                                             // Circulation.h
    int calculateFine(time_t issueTime, time_t returnTime) const;  // Circulation.h

    // ---------- Storage (owner: Shushant) ----------
    bool saveToFile() const;   // Storage.h
    bool loadFromFile();       // Storage.h
};

#endif

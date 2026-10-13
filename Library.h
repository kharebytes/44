#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <algorithm>
#include "Book.h"
#include "Input.h"
using namespace std;

class Library {
private:
    vector<Book> books;

public:
    // Returns the position of the book in the vector, or -1 if not found.
    int findIndexById(int id) const {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == id) return (int)i;
        }
        return -1;
    }

    // Returns a pointer to the book with this ID, or nullptr.
    // (Issue/return can use this to change the quantity.)
    Book* getBookById(int id) {
        int i = findIndexById(id);
        return (i == -1) ? nullptr : &books[i];
    }

    const vector<Book>& getBooks() const { return books; }

    // Adds a book object directly (also useful when loading from a file).
    bool addBook(const Book& b) {
        if (findIndexById(b.getId()) != -1) return false;
        books.push_back(b);
        return true;
    }

    // Asks the user for the details and adds a new book.
    void addBookFromInput() {
        int id = readInt("Book ID: ");
        if (findIndexById(id) != -1) {
            cout << "A book with this ID already exists." << endl;
            return;
        }
        string title = readLine("Title: ");
        string author = readLine("Author: ");
        int qty = readInt("Quantity: ");
        if (qty < 0) {
            cout << "Quantity cannot be negative." << endl;
            return;
        }
        string section = readLine("Section (e.g. CSE): ");
        int rack = readInt("Rack number: ");
        int shelf = readInt("Shelf number: ");

        addBook(Book(id, title, author, qty, section, rack, shelf));
        cout << "Book added successfully." << endl;
    }

    // Search by ID, title or author. Every result shows the location.
    void searchBook() const {
        string key = readLine("Search by ID, title or author: ");
        bool isNumber = !key.empty() && key.size() <= 9 &&
                        all_of(key.begin(), key.end(), ::isdigit);
        int idKey = isNumber ? stoi(key) : -1;

        int count = 0;
        for (const Book& b : books) {
            if (b.matches(key) || (isNumber && b.getId() == idKey)) {
                cout << "------------------------------" << endl;
                b.display();
                count++;
            }
        }
        if (count == 0) cout << "No book found." << endl;
        else cout << "------------------------------\n" << count << " book(s) found." << endl;
    }

    // TODO (teammates): issueBook(), returnBook(), calculateFine(),
    // registerMember(), saveToFile(), loadFromFile()
};

#endif

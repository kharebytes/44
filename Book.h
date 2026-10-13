#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Book {
private:
    int id;
    string title, author;
    int quantity;
    string section;
    int rack, shelf;

    static string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    Book() : id(0), quantity(0), rack(0), shelf(0) {}
    Book(int id, string title, string author, int quantity,
         string section, int rack, int shelf)
        : id(id), title(title), author(author), quantity(quantity),
          section(section), rack(rack), shelf(shelf) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getQuantity() const { return quantity; }
    string getSection() const { return section; }
    int getRack() const { return rack; }
    int getShelf() const { return shelf; }

    void setQuantity(int q) { quantity = q; }

    // true if the keyword is found in title or author (ignores case)
    bool matches(const string& key) const {
        string k = toLower(key);
        return toLower(title).find(k) != string::npos ||
               toLower(author).find(k) != string::npos;
    }

    void display() const {
        cout << "ID: " << id << " | " << title << " by " << author
             << " | Qty: " << quantity << "\n"
             << "Location: Section " << section << " > Rack " << rack
             << " > Shelf " << shelf << endl;
    }
};

#endif

#ifndef STORAGE_H
#define STORAGE_H
// Owner: Shushant. Saves and loads library data using text files.
#include "Library.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <cerrno>
#include <cstdlib>

// Split one line using the required '|' separator.
inline vector<string> splitStorageLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream input(line);
    while (getline(input, field, '|')) {
        fields.push_back(field);
    }
    // Preserve an empty final field, which getline does not return.
    if (!line.empty() && line[line.size() - 1] == '|') fields.push_back("");
    return fields;
}

inline bool storageInt(const string& text, int& value) {
    if (text.empty()) return false;
    stringstream input(text);
    input >> value;
    return !input.fail() && input.peek() == char_traits<char>::eof();
}

inline bool storageTime(const string& text, time_t& value) {
    if (text.empty()) return false;
    errno = 0;
    char* end = nullptr;
    long long parsed = strtoll(text.c_str(), &end, 10);
    if (errno == ERANGE || end == text.c_str() || *end != '\0') return false;
    value = static_cast<time_t>(parsed);
    // Ensure conversion to time_t did not change the value.
    if (static_cast<long long>(value) != parsed) return false;
    return true;
}

inline bool Library::saveToFile() const {
    ofstream booksFile("books.txt", ios::out | ios::trunc);
    ofstream membersFile("members.txt", ios::out | ios::trunc);
    ofstream issuesFile("issues.txt", ios::out | ios::trunc);

    if (!booksFile.is_open() || !membersFile.is_open() || !issuesFile.is_open()) {
        cout << "Could not open one or more data files for saving." << endl;
        return false;
    }

    const vector<Book>& allBooks = getBooks();
    for (size_t i = 0; i < allBooks.size(); i++) {
        const Book& book = allBooks[i];
        booksFile << book.getId() << '|' << cleanText(book.getTitle()) << '|'
                  << cleanText(book.getAuthor()) << '|' << book.getQuantity() << '|'
                  << cleanText(book.getSection()) << '|' << book.getRack() << '|'
                  << book.getShelf() << '\n';
    }

    const map<int, Member>& allMembers = getMembers();
    for (map<int, Member>::const_iterator it = allMembers.begin(); it != allMembers.end(); ++it) {
        const Member& member = it->second;
        membersFile << member.getId() << '|' << cleanText(member.getName()) << '\n';
        const vector<IssueRecord>& issued = member.getIssued();
        for (size_t i = 0; i < issued.size(); i++) {
            issuesFile << member.getId() << '|' << issued[i].bookId << '|'
                       << static_cast<long long>(issued[i].issueTime) << '\n';
        }
    }

    booksFile.close();
    membersFile.close();
    issuesFile.close();
    if (!booksFile || !membersFile || !issuesFile) {
        cout << "An error occurred while writing the data files." << endl;
        return false;
    }
    return true;
}

inline bool Library::loadFromFile() {
    string line;
    int lineNumber = 0;

    // A missing file is allowed: that part of the library starts empty.
    ifstream booksFile("books.txt");
    if (booksFile.is_open()) {
        lineNumber = 0;
        while (getline(booksFile, line)) {
            lineNumber++;
            vector<string> f = splitStorageLine(line);
            int id, quantity, rack, shelf;
            if (f.size() != 7 || !storageInt(f[0], id) || id <= 0 ||
                f[1].empty() || f[2].empty() || !storageInt(f[3], quantity) || quantity < 0 ||
                f[4].empty() || !storageInt(f[5], rack) || rack < 0 ||
                !storageInt(f[6], shelf) || shelf < 0) {
                cout << "Skipping bad line " << lineNumber << " in books.txt." << endl;
                continue;
            }
            if (!addBook(Book(id, f[1], f[2], quantity, f[4], rack, shelf))) {
                cout << "Skipping duplicate book ID on line " << lineNumber << " in books.txt." << endl;
            }
        }
        booksFile.close();
    }

    ifstream membersFile("members.txt");
    if (membersFile.is_open()) {
        lineNumber = 0;
        while (getline(membersFile, line)) {
            lineNumber++;
            vector<string> f = splitStorageLine(line);
            int id;
            if (f.size() != 2 || !storageInt(f[0], id) || id <= 0 || f[1].empty()) {
                cout << "Skipping bad line " << lineNumber << " in members.txt." << endl;
                continue;
            }
            if (!addMember(Member(id, f[1]))) {
                cout << "Skipping duplicate member ID on line " << lineNumber << " in members.txt." << endl;
            }
        }
        membersFile.close();
    }

    ifstream issuesFile("issues.txt");
    if (issuesFile.is_open()) {
        lineNumber = 0;
        while (getline(issuesFile, line)) {
            lineNumber++;
            vector<string> f = splitStorageLine(line);
            int memberId, bookId;
            time_t issueTime;
            if (f.size() != 3 || !storageInt(f[0], memberId) || memberId <= 0 ||
                !storageInt(f[1], bookId) || bookId <= 0 || !storageTime(f[2], issueTime)) {
                cout << "Skipping bad line " << lineNumber << " in issues.txt." << endl;
                continue;
            }
            Member* member = getMemberById(memberId);
            if (member == nullptr || getBookById(bookId) == nullptr || member->hasIssued(bookId)) {
                cout << "Skipping invalid issue record on line " << lineNumber << " in issues.txt." << endl;
                continue;
            }
            member->addIssue(bookId, issueTime);
        }
        issuesFile.close();
    }
    return true;
}

#endif

#ifndef MEMBER_H
#define MEMBER_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// One issued book of a member: which book and when it was issued.
struct IssueRecord {
    int bookId;
    time_t issueTime;
};

class Member {
private:
    int id;
    string name;
    vector<IssueRecord> issued;

public:
    Member() : id(0) {}
    Member(int id, string name) : id(id), name(name) {}

    int getId() const { return id; }
    string getName() const { return name; }
    const vector<IssueRecord>& getIssued() const { return issued; }

    void addIssue(int bookId, time_t issueTime) {
        IssueRecord r;
        r.bookId = bookId;
        r.issueTime = issueTime;
        issued.push_back(r);
    }

    bool hasIssued(int bookId) const {
        for (size_t i = 0; i < issued.size(); i++) {
            if (issued[i].bookId == bookId) return true;
        }
        return false;
    }

    // Removes the issue record of this book. Returns false if the member
    // does not have it. The removed record is copied into 'removed'.
    bool removeIssue(int bookId, IssueRecord& removed) {
        for (size_t i = 0; i < issued.size(); i++) {
            if (issued[i].bookId == bookId) {
                removed = issued[i];
                issued.erase(issued.begin() + i);
                return true;
            }
        }
        return false;
    }

    void display() const {
        cout << "ID: " << id << " | " << name
             << " | Books issued: " << issued.size() << endl;
    }
};

#endif

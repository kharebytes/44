#ifndef CIRCULATION_H
#define CIRCULATION_H

// Owner: Srajen. Handles issuing, returning, dates and fines.
#include "Library.h"
#include <iomanip>
#include <sstream>
#include <ctime>

inline bool parseCirculationDate(const string& text, time_t& result) {
    // Expected format: DD-MM-YYYY
    if (text.size() != 10 || text[2] != '-' || text[5] != '-') {
        return false;
    }

    for (size_t i = 0; i < text.size(); i++) {
        if (i == 2 || i == 5) continue;
        if (text[i] < '0' || text[i] > '9') return false;
    }

    int day = stoi(text.substr(0, 2));
    int month = stoi(text.substr(3, 2));
    int year = stoi(text.substr(6, 4));

    if (month < 1 || month > 12 || day < 1 || day > 31 ||
        year < 1900 || year > 9999) {
        return false;
    }

    tm dateParts = {};
    dateParts.tm_mday = day;
    dateParts.tm_mon = month - 1;
    dateParts.tm_year = year - 1900;
    dateParts.tm_hour = 0; // Store a date-only issue time at local midnight.
    dateParts.tm_isdst = -1;

    time_t parsed = mktime(&dateParts);
    if (parsed == static_cast<time_t>(-1)) return false;

    // mktime adjusts impossible dates, so compare the adjusted date.
    tm* checked = localtime(&parsed);
    if (checked == nullptr ||
        checked->tm_mday != day ||
        checked->tm_mon != month - 1 ||
        checked->tm_year != year - 1900) {
        return false;
    }

    result = parsed;
    return true;
}

inline string formatCirculationDate(time_t value) {
    tm* dateParts = localtime(&value);
    if (dateParts == nullptr) return "Unknown date";

    char buffer[11];
    if (strftime(buffer, sizeof(buffer), "%d-%m-%Y", dateParts) == 0) {
        return "Unknown date";
    }
    return string(buffer);
}

inline time_t startOfTodayForCirculation() {
    time_t now = time(nullptr);
    tm* current = localtime(&now);
    if (current == nullptr) return now;

    tm today = *current;
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;
    today.tm_isdst = -1;
    return mktime(&today);
}

inline time_t addCirculationDays(time_t value, int days) {
    tm* dateParts = localtime(&value);
    if (dateParts == nullptr) return value;

    tm due = *dateParts;
    due.tm_mday += days;
    due.tm_isdst = -1;
    return mktime(&due);
}

inline void Library::issueBook() {
    int memberId = readInt("Member ID: ");
    Member* member = getMemberById(memberId);
    if (member == nullptr) {
        cout << "Member not found." << endl;
        return;
    }

    int bookId = readInt("Book ID: ");
    Book* book = getBookById(bookId);
    if (book == nullptr) {
        cout << "Book not found." << endl;
        return;
    }

    if (book->getQuantity() <= 0) {
        cout << "No copies available." << endl;
        return;
    }

    if (member->hasIssued(bookId)) {
        cout << "This member has already issued this book." << endl;
        return;
    }

    string dateText = readLine("Issue date (DD-MM-YYYY) or press Enter for today: ");
    time_t issueTime;
    if (dateText.empty()) {
        issueTime = time(nullptr);
    } else if (!parseCirculationDate(dateText, issueTime)) {
        cout << "Invalid date. Use DD-MM-YYYY and enter a real calendar date." << endl;
        return;
    }

    if (dateText.empty()) {
        // Keep today's actual time when the user chooses the default.
    } else if (issueTime > startOfTodayForCirculation()) {
        cout << "Issue date cannot be in the future." << endl;
        return;
    }

    book->setQuantity(book->getQuantity() - 1);
    member->addIssue(bookId, issueTime);

    cout << "Book issued successfully." << endl;
    cout << "Issue date: " << formatCirculationDate(issueTime) << endl;
    cout << "Due date: " << formatCirculationDate(addCirculationDays(issueTime, ALLOWED_DAYS)) << endl;
}

inline int Library::calculateFine(time_t issueTime, time_t returnTime) const {
    if (returnTime <= issueTime) return 0;

    double secondsKept = difftime(returnTime, issueTime);
    int daysKept = static_cast<int>(secondsKept / (60 * 60 * 24));
    if (daysKept <= ALLOWED_DAYS) return 0;

    return (daysKept - ALLOWED_DAYS) * FINE_PER_DAY;
}

inline void Library::returnBook() {
    int memberId = readInt("Member ID: ");
    Member* member = getMemberById(memberId);
    if (member == nullptr) {
        cout << "Member not found." << endl;
        return;
    }

    int bookId = readInt("Book ID: ");
    Book* book = getBookById(bookId);
    if (book == nullptr) {
        cout << "Book not found." << endl;
        return;
    }

    IssueRecord removed;
    if (!member->removeIssue(bookId, removed)) {
        cout << "This member has not issued this book." << endl;
        return;
    }

    time_t returnTime = time(nullptr);
    double secondsKept = difftime(returnTime, removed.issueTime);
    int daysKept = secondsKept <= 0 ? 0 :
                   static_cast<int>(secondsKept / (60 * 60 * 24));
    int fine = calculateFine(removed.issueTime, returnTime);

    book->setQuantity(book->getQuantity() + 1);

    cout << "Book returned successfully." << endl;
    cout << "Days kept: " << daysKept << endl;
    cout << "Fine: Rs. " << fine << endl;
    cout << "Place the book at: Section " << book->getSection()
         << " > Rack " << book->getRack()
         << " > Shelf " << book->getShelf() << endl;
}

#endif

#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Reads one full line after showing a prompt.
inline string readLine(const string& prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {          // input ended (Ctrl+Z / Ctrl+D)
        cout << "\nInput ended. Exiting." << endl;
        exit(0);
    }
    return s;
}

// Keeps asking until the user enters a valid whole number.
inline int readInt(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos;
            int v = stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {
        }
        cout << "Please enter a valid number." << endl;
    }
}

// Text is saved in files with '|' as separator, so '|' is not allowed
// inside a title, author, section or name. Replace it with '/'.
inline string cleanText(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '|') s[i] = '/';
    }
    return s;
}

#endif

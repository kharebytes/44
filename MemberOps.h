
#ifndef MEMBEROPS_H
#define MEMBEROPS_H

#include "Library.h"

inline void Library::registerMemberFromInput()
{
    int id = readInt("Enter member ID: ");

    if (id <= 0)
    {
        cout << "Member ID must be positive." << endl;
        return;
    }

    if (getMemberById(id) != nullptr)
    {
        cout << "Member ID already exists." << endl;
        return;
    }

    string name = cleanText(readLine("Enter member name: "));

    if (name.empty())
    {
        cout << "Member name cannot be empty." << endl;
        return;
    }

    addMember(Member(id, name));
    cout << "Member registered successfully." << endl;
}

inline void Library::viewMembers() const
{
    const map<int, Member>& members = getMembers();

    if (members.empty())
    {
        cout << "No members registered." << endl;
        return;
    }

    for (map<int, Member>::const_iterator it = members.begin();
         it != members.end(); ++it)
    {
        it->second.display();
    }

    cout << "Total members: " << members.size() << endl;
}

#endif


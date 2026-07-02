#include "../include/Member.h"
#include <iostream>
#include <sstream>

Member::Member() : id(0), name(""), email("") {}

Member::Member(int id, string name, string email)
    : id(id), name(name), email(email) {}

void Member::display() const {
    cout << "ID: " << id
         << " | Name: " << name
         << " | Email: " << email
         << " | Borrowed Books: " << borrowedBooks.size() << endl;
}

string Member::toFileString() const {
    ostringstream oss;
    oss << id << "|" << name << "|" << email << "|";
    for (size_t i = 0; i < borrowedBooks.size(); ++i) {
        oss << borrowedBooks[i];
        if (i != borrowedBooks.size() - 1) oss << ",";
    }
    return oss.str();
}

Member Member::fromFileString(const string &line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    Member m;
    if (fields.size() >= 3) {
        m.id = stoi(fields[0]);
        m.name = fields[1];
        m.email = fields[2];
    }
    if (fields.size() >= 4 && !fields[3].empty()) {
        stringstream bs(fields[3]);
        string isbn;
        while (getline(bs, isbn, ',')) {
            m.borrowedBooks.push_back(isbn);
        }
    }
    return m;
}

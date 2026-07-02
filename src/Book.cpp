#include "../include/Book.h"
#include <iostream>
#include <sstream>
#include <vector>

Book::Book() : isbn(""), title(""), author(""), totalCopies(0), availableCopies(0) {}

Book::Book(string isbn, string title, string author, int totalCopies)
    : isbn(isbn), title(title), author(author),
      totalCopies(totalCopies), availableCopies(totalCopies) {}

void Book::display() const {
    cout << "ISBN: " << isbn
         << " | Title: " << title
         << " | Author: " << author
         << " | Total Copies: " << totalCopies
         << " | Available: " << availableCopies << endl;
}

string Book::toFileString() const {
    ostringstream oss;
    oss << isbn << "|" << title << "|" << author << "|"
        << totalCopies << "|" << availableCopies;
    return oss.str();
}

Book Book::fromFileString(const string &line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    Book b;
    if (fields.size() >= 5) {
        b.isbn = fields[0];
        b.title = fields[1];
        b.author = fields[2];
        b.totalCopies = stoi(fields[3]);
        b.availableCopies = stoi(fields[4]);
    }
    return b;
}

#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
public:
    string isbn;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Book();
    Book(string isbn, string title, string author, int totalCopies);

    void display() const;

    // Serialize / deserialize for simple file persistence (pipe-delimited)
    string toFileString() const;
    static Book fromFileString(const string &line);
};

#endif // BOOK_H

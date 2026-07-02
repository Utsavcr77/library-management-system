#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include "Book.h"
#include "Member.h"
using namespace std;

struct IssueRecord {
    string isbn;
    int memberId;
    string issueDate;
    string dueDate;
    bool returned;

    string toFileString() const;
    static IssueRecord fromFileString(const string &line);
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<IssueRecord> issueRecords;

    const string booksFile   = "data/books.txt";
    const string membersFile = "data/members.txt";
    const string issuesFile  = "data/issues.txt";

public:
    // Book management
    void addBook(const string &isbn, const string &title,
                 const string &author, int copies);
    bool removeBook(const string &isbn);
    Book* findBookByISBN(const string &isbn);
    void displayAllBooks() const;

    // Member management
    void addMember(const string &name, const string &email);
    Member* findMemberById(int id);
    void displayAllMembers() const;

    // Issue / Return
    bool issueBook(const string &isbn, int memberId);
    bool returnBook(const string &isbn, int memberId);
    void displayIssuedBooks() const;

    // Search
    void searchByTitle(const string &title) const;
    void searchByAuthor(const string &author) const;

    // Persistence
    void loadData();
    void saveData();

    int nextMemberId() const;
};

#endif // LIBRARY_H

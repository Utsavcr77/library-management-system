#include "../include/Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

// ---------- IssueRecord helpers ----------

string IssueRecord::toFileString() const {
    ostringstream oss;
    oss << isbn << "|" << memberId << "|" << issueDate << "|"
        << dueDate << "|" << (returned ? 1 : 0);
    return oss.str();
}

IssueRecord IssueRecord::fromFileString(const string &line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    IssueRecord r;
    if (fields.size() >= 5) {
        r.isbn = fields[0];
        r.memberId = stoi(fields[1]);
        r.issueDate = fields[2];
        r.dueDate = fields[3];
        r.returned = (fields[4] == "1");
    }
    return r;
}

// ---------- Date helper ----------

static string dateNowPlusDays(int daysToAdd) {
    time_t now = time(nullptr);
    now += daysToAdd * 24 * 60 * 60;
    tm *ltm = localtime(&now);

    ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-";
    if (ltm->tm_mon + 1 < 10) oss << "0";
    oss << (ltm->tm_mon + 1) << "-";
    if (ltm->tm_mday < 10) oss << "0";
    oss << ltm->tm_mday;
    return oss.str();
}

// ---------- Book management ----------

void Library::addBook(const string &isbn, const string &title,
                       const string &author, int copies) {
    Book *existing = findBookByISBN(isbn);
    if (existing != nullptr) {
        // Same ISBN already exists -> just add more copies
        existing->totalCopies += copies;
        existing->availableCopies += copies;
        cout << "Existing book found. Added " << copies
             << " more copies of \"" << existing->title << "\".\n";
        return;
    }
    books.push_back(Book(isbn, title, author, copies));
    cout << "Book \"" << title << "\" added successfully.\n";
}

bool Library::removeBook(const string &isbn) {
    auto it = remove_if(books.begin(), books.end(),
                         [&](const Book &b) { return b.isbn == isbn; });
    if (it == books.end()) return false;
    books.erase(it, books.end());
    return true;
}

Book* Library::findBookByISBN(const string &isbn) {
    for (auto &b : books) {
        if (b.isbn == isbn) return &b;
    }
    return nullptr;
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        cout << "No books in the library yet.\n";
        return;
    }
    cout << "\n--- All Books ---\n";
    for (const auto &b : books) {
        b.display();
    }
}

// ---------- Member management ----------

int Library::nextMemberId() const {
    int maxId = 0;
    for (const auto &m : members) {
        if (m.id > maxId) maxId = m.id;
    }
    return maxId + 1;
}

void Library::addMember(const string &name, const string &email) {
    int id = nextMemberId();
    members.push_back(Member(id, name, email));
    cout << "Member \"" << name << "\" registered with ID: " << id << endl;
}

Member* Library::findMemberById(int id) {
    for (auto &m : members) {
        if (m.id == id) return &m;
    }
    return nullptr;
}

void Library::displayAllMembers() const {
    if (members.empty()) {
        cout << "No members registered yet.\n";
        return;
    }
    cout << "\n--- All Members ---\n";
    for (const auto &m : members) {
        m.display();
    }
}

// ---------- Issue / Return ----------

bool Library::issueBook(const string &isbn, int memberId) {
    Book *book = findBookByISBN(isbn);
    Member *member = findMemberById(memberId);

    if (book == nullptr) {
        cout << "Error: Book with ISBN " << isbn << " not found.\n";
        return false;
    }
    if (member == nullptr) {
        cout << "Error: Member with ID " << memberId << " not found.\n";
        return false;
    }
    if (book->availableCopies <= 0) {
        cout << "Error: No available copies of \"" << book->title << "\" right now.\n";
        return false;
    }

    IssueRecord record;
    record.isbn = isbn;
    record.memberId = memberId;
    record.issueDate = dateNowPlusDays(0);
    record.dueDate = dateNowPlusDays(14); // 2-week borrowing period
    record.returned = false;

    issueRecords.push_back(record);
    book->availableCopies--;
    member->borrowedBooks.push_back(isbn);

    cout << "Book \"" << book->title << "\" issued to " << member->name
         << ". Due date: " << record.dueDate << endl;
    return true;
}

bool Library::returnBook(const string &isbn, int memberId) {
    Book *book = findBookByISBN(isbn);
    Member *member = findMemberById(memberId);

    if (book == nullptr) {
        cout << "Error: Book with ISBN " << isbn << " not found.\n";
        return false;
    }
    if (member == nullptr) {
        cout << "Error: Member with ID " << memberId << " not found.\n";
        return false;
    }

    // Find an active (not-yet-returned) issue record for this book+member
    IssueRecord *activeRecord = nullptr;
    for (auto &r : issueRecords) {
        if (r.isbn == isbn && r.memberId == memberId && !r.returned) {
            activeRecord = &r;
            break;
        }
    }

    if (activeRecord == nullptr) {
        cout << "Error: No active borrow record found for this book/member pair.\n";
        return false;
    }

    activeRecord->returned = true;
    book->availableCopies++;

    auto &bb = member->borrowedBooks;
    bb.erase(remove(bb.begin(), bb.end(), isbn), bb.end());

    cout << "Book \"" << book->title << "\" returned by " << member->name << ".\n";
    return true;
}

void Library::displayIssuedBooks() const {
    bool any = false;
    cout << "\n--- Currently Issued Books ---\n";
    for (const auto &r : issueRecords) {
        if (!r.returned) {
            any = true;
            cout << "ISBN: " << r.isbn
                 << " | Member ID: " << r.memberId
                 << " | Issued: " << r.issueDate
                 << " | Due: " << r.dueDate << endl;
        }
    }
    if (!any) cout << "No books are currently issued.\n";
}

// ---------- Search ----------

void Library::searchByTitle(const string &title) const {
    string needle = title;
    transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

    bool found = false;
    cout << "\n--- Search Results (Title contains \"" << title << "\") ---\n";
    for (const auto &b : books) {
        string haystack = b.title;
        transform(haystack.begin(), haystack.end(), haystack.begin(), ::tolower);
        if (haystack.find(needle) != string::npos) {
            b.display();
            found = true;
        }
    }
    if (!found) cout << "No books matched that title.\n";
}

void Library::searchByAuthor(const string &author) const {
    string needle = author;
    transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

    bool found = false;
    cout << "\n--- Search Results (Author contains \"" << author << "\") ---\n";
    for (const auto &b : books) {
        string haystack = b.author;
        transform(haystack.begin(), haystack.end(), haystack.begin(), ::tolower);
        if (haystack.find(needle) != string::npos) {
            b.display();
            found = true;
        }
    }
    if (!found) cout << "No books matched that author.\n";
}

// ---------- Persistence ----------

void Library::loadData() {
    books.clear();
    members.clear();
    issueRecords.clear();

    ifstream bf(booksFile);
    string line;
    if (bf.is_open()) {
        while (getline(bf, line)) {
            if (!line.empty()) books.push_back(Book::fromFileString(line));
        }
        bf.close();
    }

    ifstream mf(membersFile);
    if (mf.is_open()) {
        while (getline(mf, line)) {
            if (!line.empty()) members.push_back(Member::fromFileString(line));
        }
        mf.close();
    }

    ifstream isf(issuesFile);
    if (isf.is_open()) {
        while (getline(isf, line)) {
            if (!line.empty()) issueRecords.push_back(IssueRecord::fromFileString(line));
        }
        isf.close();
    }
}

void Library::saveData() {
    ofstream bf(booksFile);
    for (const auto &b : books) bf << b.toFileString() << "\n";
    bf.close();

    ofstream mf(membersFile);
    for (const auto &m : members) mf << m.toFileString() << "\n";
    mf.close();

    ofstream isf(issuesFile);
    for (const auto &r : issueRecords) isf << r.toFileString() << "\n";
    isf.close();
}

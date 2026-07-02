#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>
using namespace std;

class Member {
public:
    int id;
    string name;
    string email;
    vector<string> borrowedBooks; // stores ISBNs currently borrowed

    Member();
    Member(int id, string name, string email);

    void display() const;

    string toFileString() const;
    static Member fromFileString(const string &line);
};

#endif // MEMBER_H

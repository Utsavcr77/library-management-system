#include <iostream>
#include <limits>
#include "../include/Library.h"
using namespace std;

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static void printMenu() {
    cout << "\n===================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM\n";
    cout << "===================================\n";
    cout << " 1. Add Book\n";
    cout << " 2. Register Member\n";
    cout << " 3. Issue Book\n";
    cout << " 4. Return Book\n";
    cout << " 5. Search Book by Title\n";
    cout << " 6. Search Book by Author\n";
    cout << " 7. Display All Books\n";
    cout << " 8. Display All Members\n";
    cout << " 9. Display Currently Issued Books\n";
    cout << " 0. Save & Exit\n";
    cout << "===================================\n";
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    library.loadData();

    int choice;
    do {
        printMenu();
        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        clearInput();

        switch (choice) {
            case 1: {
                string isbn, title, author;
                int copies;
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter Title: ";
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter number of copies: ";
                cin >> copies;
                clearInput();
                library.addBook(isbn, title, author, copies);
                break;
            }
            case 2: {
                string name, email;
                cout << "Enter Member Name: ";
                getline(cin, name);
                cout << "Enter Member Email: ";
                getline(cin, email);
                library.addMember(name, email);
                break;
            }
            case 3: {
                string isbn;
                int memberId;
                cout << "Enter ISBN of book to issue: ";
                getline(cin, isbn);
                cout << "Enter Member ID: ";
                cin >> memberId;
                clearInput();
                library.issueBook(isbn, memberId);
                break;
            }
            case 4: {
                string isbn;
                int memberId;
                cout << "Enter ISBN of book to return: ";
                getline(cin, isbn);
                cout << "Enter Member ID: ";
                cin >> memberId;
                clearInput();
                library.returnBook(isbn, memberId);
                break;
            }
            case 5: {
                string title;
                cout << "Enter title (or part of it) to search: ";
                getline(cin, title);
                library.searchByTitle(title);
                break;
            }
            case 6: {
                string author;
                cout << "Enter author (or part of it) to search: ";
                getline(cin, author);
                library.searchByAuthor(author);
                break;
            }
            case 7:
                library.displayAllBooks();
                break;
            case 8:
                library.displayAllMembers();
                break;
            case 9:
                library.displayIssuedBooks();
                break;
            case 0:
                library.saveData();
                cout << "Data saved. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

# Library Management System (C++)

A console-based Library Management System built in C++ using object-oriented
programming. It manages books, members, and borrowing records, with simple
file-based persistence so data survives between runs.

## Features

- **Book management** — add books (with ISBN, title, author, copy count);
  duplicate ISBNs automatically increase the copy count.
- **Member management** — register members with an auto-generated ID.
- **Issue / Return** — issue a book to a member (tracks issue date and a
  14-day due date) and process returns, updating available copy counts.
- **Search** — case-insensitive partial-match search by title or by author.
- **Persistence** — all data is saved to plain text files in `data/` on exit
  and reloaded automatically on the next run.

## Project Structure

```
LibraryManagementSystem/
├── include/
│   ├── Book.h
│   ├── Member.h
│   └── Library.h
├── src/
│   ├── Book.cpp
│   ├── Member.cpp
│   ├── Library.cpp
│   └── main.cpp
├── data/              # created/updated at runtime (books.txt, members.txt, issues.txt)
├── Makefile
├── .gitignore
└── README.md
```

## Design Overview

- `Book` — represents a book (ISBN, title, author, total/available copies).
- `Member` — represents a library member (ID, name, email, borrowed ISBNs).
- `IssueRecord` (inside `Library.h`) — represents one issue/return transaction.
- `Library` — the central engine that owns all books, members, and issue
  records, and exposes the operations used by `main.cpp`'s menu.

## Building & Running

### Option 1: Using `make`

```bash
make        # builds the `library_system` executable
make run    # builds (if needed) and runs it
make clean  # removes the compiled binary
```

### Option 2: Compiling directly with g++

```bash
g++ -std=c++11 -Wall -Iinclude src/main.cpp src/Book.cpp src/Member.cpp src/Library.cpp -o library_system
./library_system
```

> On Windows, run `library_system.exe` instead of `./library_system`.

## Usage

Run the program and use the on-screen menu:

```
===================================
   LIBRARY MANAGEMENT SYSTEM
===================================
 1. Add Book
 2. Register Member
 3. Issue Book
 4. Return Book
 5. Search Book by Title
 6. Search Book by Author
 7. Display All Books
 8. Display All Members
 9. Display Currently Issued Books
 0. Save & Exit
===================================
```

All data is written to `data/books.txt`, `data/members.txt`, and
`data/issues.txt` when you choose option `0` (Save & Exit).

## Possible Future Enhancements

- Overdue fine calculation based on due dates.
- Multiple simultaneous copies tracked per physical item (barcodes).
- A simple GUI (Qt) or web front-end on top of the same `Library` engine.

## License

This project is open source and available for educational use.

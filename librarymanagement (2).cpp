#include <iostream>
#include <string>
using namespace std;

const int LOAN_DAYS = 7;      // days a book can be kept without fine
const int FINE_PER_DAY = 2;   // fine in Rs. for every extra day

// ---------------------------------------------------------------
// Class Book
// ---------------------------------------------------------------
class Book {
private:
    int id;
    string title;
    string author;
    bool issued;
    static int count;

public:
    Book() {
        id = 0;
        title = "";
        author = "";
        issued = false;
        count++;
    }

    void input() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();                 // clear the newline before getline

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        issued = false;
    }

    void setTitle(string t) {
        title = t;
    }

    void setAuthor(string a) {
        author = a;
    }

    void setIssued(bool status) {
        issued = status;
    }

    int getId() {
        return id;
    }

    string getTitle() {
        return title;
    }

    string getAuthor() {
        return author;
    }

    bool isIssued() {
        return issued;
    }

    static void showCount() {
        cout << "Total book objects: " << count << endl;
    }

    friend void showSummary(Book b);

    virtual void display() {
        cout << "Book ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
    }

    virtual ~Book() {
        count--;
    }
};

int Book::count = 0;

void showSummary(Book b) {
    cout << b.id << " - " << b.title << " by " << b.author << endl;
}

// ---------------------------------------------------------------
// Class LibraryBook (inherits Book)
// ---------------------------------------------------------------
class LibraryBook : public Book {
public:
    void display() override {
        Book::display();

        if (isIssued())
            cout << "Status: Issued\n";
        else
            cout << "Status: Available\n";
    }
};

// ---------------------------------------------------------------
// Class IssueRecord (stores one issue/return transaction)
// ---------------------------------------------------------------
class IssueRecord {
private:
    int bookId;
    string memberName;
    int daysKept;
    int fine;
    bool returned;

public:
    IssueRecord() {
        bookId = 0;
        memberName = "";
        daysKept = 0;
        fine = 0;
        returned = false;
    }

    void issue(int id, string name) {
        bookId = id;
        memberName = name;
        returned = false;
    }

    void returnBook(int days) {
        daysKept = days;
        returned = true;

        if (days > LOAN_DAYS)
            fine = (days - LOAN_DAYS) * FINE_PER_DAY;
        else
            fine = 0;
    }

    int getBookId() {
        return bookId;
    }

    int getFine() {
        return fine;
    }

    bool isReturned() {
        return returned;
    }

    void display() {
        cout << "Book ID: " << bookId << ", Member: " << memberName;

        if (returned)
            cout << ", Days kept: " << daysKept << ", Fine: Rs." << fine << endl;
        else
            cout << ", Not returned yet\n";
    }
};

// ---------------------------------------------------------------
// Function to find a book by ID (returns -1 if not found)
// ---------------------------------------------------------------
int findBook(Book* books[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (books[i]->getId() == id)
            return i;
    }
    return -1;
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main() {
    const int MAX = 100;
    Book* books[MAX];
    IssueRecord records[MAX];
    int count = 0;        // number of books
    int recCount = 0;     // number of transactions
    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Update Book\n";
        cout << "3. Delete Book\n";
        cout << "4. Display All Books\n";
        cout << "5. Search Book\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book (Calculate Fine)\n";
        cout << "8. Generate Report\n";
        cout << "9. Show Total Objects\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            // ----- Module 1: Add book -----
            case 1: {
                if (count == MAX) {
                    cout << "Library is full!\n";
                    break;
                }

                Book* b = new LibraryBook();
                b->input();

                if (findBook(books, count, b->getId()) != -1) {
                    cout << "Book ID already exists!\n";
                    delete b;
                } else {
                    books[count] = b;
                    count++;
                    cout << "Book added successfully!\n";
                }
                break;
            }

            // ----- Module 1: Update book -----
            case 2: {
                int id;
                cout << "Enter Book ID to update: ";
                cin >> id;

                int found = findBook(books, count, id);

                if (found == -1) {
                    cout << "Book not found!\n";
                } else {
                    string t, a;
                    cin.ignore();

                    cout << "Enter new title: ";
                    getline(cin, t);
                    cout << "Enter new author: ";
                    getline(cin, a);

                    books[found]->setTitle(t);
                    books[found]->setAuthor(a);
                    cout << "Book updated successfully!\n";
                }
                break;
            }

            // ----- Module 1: Delete book -----
            case 3: {
                int id;
                cout << "Enter Book ID to delete: ";
                cin >> id;

                int found = findBook(books, count, id);

                if (found == -1) {
                    cout << "Book not found!\n";
                } else if (books[found]->isIssued()) {
                    cout << "Book is issued, cannot delete!\n";
                } else {
                    delete books[found];

                    for (int i = found; i < count - 1; i++) {
                        books[i] = books[i + 1];
                    }

                    count--;
                    cout << "Book deleted successfully!\n";
                }
                break;
            }

            // ----- Module 2: Display all books -----
            case 4:
                if (count == 0) {
                    cout << "No book records found!\n";
                }

                for (int i = 0; i < count; i++) {
                    cout << "\nBook " << i + 1 << endl;
                    books[i]->display();
                }
                break;

            // ----- Module 2: Search book -----
            case 5: {
                int option;
                cout << "1. Search by ID\n2. Search by Title\n";
                cout << "Enter option: ";
                cin >> option;

                if (option == 1) {
                    int id;
                    cout << "Enter Book ID: ";
                    cin >> id;

                    int found = findBook(books, count, id);

                    if (found == -1)
                        cout << "Book not found!\n";
                    else
                        books[found]->display();

                } else if (option == 2) {
                    string key;
                    bool any = false;

                    cin.ignore();
                    cout << "Enter title to search: ";
                    getline(cin, key);

                    for (int i = 0; i < count; i++) {
                        if (books[i]->getTitle() == key) {
                            books[i]->display();
                            any = true;
                        }
                    }

                    if (!any)
                        cout << "Book not found!\n";
                } else {
                    cout << "Invalid option!\n";
                }
                break;
            }

            // ----- Module 3: Issue book -----
            case 6: {
                int id;
                cout << "Enter Book ID to issue: ";
                cin >> id;

                int found = findBook(books, count, id);

                if (found == -1) {
                    cout << "Book not found!\n";
                } else if (books[found]->isIssued()) {
                    cout << "Book is already issued!\n";
                } else {
                    string name;
                    cin.ignore();

                    cout << "Enter Member Name: ";
                    getline(cin, name);

                    records[recCount].issue(id, name);
                    recCount++;
                    books[found]->setIssued(true);
                    cout << "Book issued successfully!\n";
                    cout << "Return within " << LOAN_DAYS << " days to avoid fine.\n";
                }
                break;
            }

            // ----- Module 3 + 4: Return book and calculate fine -----
            case 7: {
                int id, days;
                cout << "Enter Book ID to return: ";
                cin >> id;

                int found = findBook(books, count, id);

                if (found == -1) {
                    cout << "Book not found!\n";
                } else if (!books[found]->isIssued()) {
                    cout << "This book is not issued!\n";
                } else {
                    cout << "Enter number of days book was kept: ";
                    cin >> days;

                    if (days < 0) {
                        cout << "Invalid days!\n";
                        break;
                    }

                    // find the pending record of this book
                    for (int i = 0; i < recCount; i++) {
                        if (records[i].getBookId() == id && !records[i].isReturned()) {
                            records[i].returnBook(days);

                            if (records[i].getFine() > 0)
                                cout << "Late return! Fine = Rs." << records[i].getFine() << endl;
                            else
                                cout << "Returned on time. No fine.\n";
                            break;
                        }
                    }

                    books[found]->setIssued(false);
                    cout << "Book returned successfully!\n";
                }
                break;
            }

            // ----- Module 4: Report -----
            case 8: {
                int issuedCount = 0, totalFine = 0;

                for (int i = 0; i < count; i++) {
                    if (books[i]->isIssued())
                        issuedCount++;
                }

                for (int i = 0; i < recCount; i++) {
                    totalFine = totalFine + records[i].getFine();
                }

                cout << "\n===== LIBRARY REPORT =====\n";
                cout << "Total books: " << count << endl;
                cout << "Available books: " << count - issuedCount << endl;
                cout << "Issued books: " << issuedCount << endl;
                cout << "Total transactions: " << recCount << endl;
                cout << "Total fine collected: Rs." << totalFine << endl;

                cout << "\n--- Transaction History ---\n";
                if (recCount == 0)
                    cout << "No transactions yet.\n";

                for (int i = 0; i < recCount; i++) {
                    cout << i + 1 << ". ";
                    records[i].display();
                }
                break;
            }

            case 9:
                Book::showCount();
                break;

            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    for (int i = 0; i < count; i++) {
        delete books[i];
    }

    return 0;
}

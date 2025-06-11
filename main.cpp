
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <unordered_map> // For std::unordered_map
#include <algorithm> // For std::sort (if you re-add sorting for viewAllBooks)

using namespace std;

// --- Book Class ---
class Book {
public:
    string title;
    string author;
    string isbn; // International Standard Book Number
    bool isIssued;

    // Original constructor
    Book(string title, string author, string isbn)
        : title(title), author(author), isbn(isbn), isIssued(false) {}

    // Default constructor needed for std::unordered_map (value type requirements)
    Book() : title(""), author(""), isbn(""), isIssued(false) {}


    void display() const {
        cout << "Title: " << title
                  << ", Author: " << author
                  << ", ISBN: " << isbn
                  << ", Status: " << (isIssued ? "Issued" : "Available") << endl;
    }

    // Removed to_string() as it was only for file saving
    // Removed from_string() as it was only for file loading
};

// --- Library Class with Unordered Map (NO DATA PERSISTENCE) ---
class Library {
private:
    // Using unordered_map for efficient ISBN-based lookups
    unordered_map<string, Book> books;
    // const string filename = "library_data.txt"; // Removed filename

    // Removed saveBooks() function
    // Removed loadBooks() function

public:
    Library() {
        // No call to loadBooks() - library starts empty each time
    }

    ~Library() {
        // No call to saveBooks() - library state is not saved
    }

    void addBook(const Book& book) {
        // Check for duplicate ISBN efficiently using find()
        if (books.find(book.isbn) != books.end()) {
            cout << "Error: Book with ISBN '" << book.isbn << "' already exists." << endl;
            return;
        }
        // Add to map
        books[book.isbn] = book;
        cout << "Book added successfully!" << endl;
    }

    void viewAllBooks() const {
        if (books.empty()) {
            cout << "No books in the library." << endl;
            return;
        }
        cout << "\n--- All Books in Library ---" << endl;
        // Iterate through the map to display all books
        // If you want sorted display, you'd need to copy to a vector and sort here.
        for (const auto& pair : books) {
            pair.second.display(); // Display the Book object (value)
        }
        cout << "--------------------------" << endl;
    }

    void searchBook(const string& query) const {
        bool found = false;
        cout << "\n--- Search Results ---" << endl;

        // First, check if it's an exact ISBN match (most efficient)
        auto it_isbn = books.find(query);
        if (it_isbn != books.end()) {
            it_isbn->second.display();
            found = true;
        } else {
            // Otherwise, iterate for title/author match
            for (const auto& pair : books) {
                if (pair.second.title.find(query) != string::npos || pair.second.author.find(query) != string::npos) {
                    // Avoid double printing if it was already found by ISBN
                    if (pair.second.isbn != query || (found && pair.second.isbn == query)) {
                        pair.second.display();
                        found = true;
                    }
                }
            }
        }

        if (!found) {
            cout << "No book found matching '" << query << "'" << endl;
        }
        cout << "--------------------" << endl;
    }

    // Helper function to get book by ISBN
    Book* getBookByIsbn(const string& isbn) {
        auto it = books.find(isbn);
        if (it != books.end()) {
            return &(it->second); // Return pointer to the Book object in the map
        }
        return nullptr; // Book not found
    }

    void issueBook(const string& isbn) {
        Book* book = getBookByIsbn(isbn);
        if (book) { // If book found
            if (!book->isIssued) {
                book->isIssued = true;
                cout << "Book '" << book->title << "' issued successfully." << endl;
            } else {
                cout << "Book '" << book->title << "' is already issued." << endl;
            }
        } else {
            cout << "Book with ISBN '" << isbn << "' not found." << endl;
        }
    }

    void returnBook(const string& isbn) {
        Book* book = getBookByIsbn(isbn);
        if (book) { // If book found
            if (book->isIssued) {
                book->isIssued = false;
                cout << "Book '" << book->title << "' returned successfully." << endl;
            } else {
                cout << "Book '" << book->title << "' was not issued." << endl;
            }
        } else {
            cout << "Book with ISBN '" << isbn << "' not found." << endl;
        }
    }

    void deleteBook(const string& isbn) {
        auto it = books.find(isbn);
        if (it != books.end()) { // If book found
            if (it->second.isIssued) {
                cout << "Cannot delete book '" << it->second.title << "'. It is currently issued." << endl;
                return;
            }
            cout << "Deleting book: '" << it->second.title << "'" << endl;
            books.erase(it); // Erase from map
            cout << "Book deleted successfully!" << endl;
        } else {
            cout << "Book with ISBN '" << isbn << "' not found." << endl;
        }
    }
};

// --- Main Function (remains the same) ---
int main() {
    Library myLibrary;
    int choice;
    string title, author, isbn, query;

    do {
        cout << "\n--- Library Management System ---" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. View All Books" << endl;
        cout << "3. Search Book" << endl;
        cout << "4. Issue Book" << endl;
        cout << "5. Return Book" << endl;
        cout << "6. Delete Book" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                cout << "Enter Title: ";
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                Book newBook(title, author, isbn);
                myLibrary.addBook(newBook);
                break;
            }
            case 2:
                myLibrary.viewAllBooks();
                break;
            case 3:
                cout << "Enter ISBN, Title, or Author to search: ";
                getline(cin, query);
                myLibrary.searchBook(query);
                break;
            case 4:
                cout << "Enter ISBN of the book to issue: ";
                getline(cin, isbn);
                myLibrary.issueBook(isbn);
                break;
            case 5:
                cout << "Enter ISBN of the book to return: ";
                getline(cin, isbn);
                myLibrary.returnBook(isbn);
                break;
            case 6:
                cout << "Enter ISBN of the book to delete: ";
                getline(cin, isbn);
                myLibrary.deleteBook(isbn);
                break;
            case 7:
                cout << "Exiting Library Management System. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 7);

    return 0;
}


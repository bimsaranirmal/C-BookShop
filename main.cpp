#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Define Book structure
struct Book {
    string title;
    string author;
    double price;
    int quantity;
};

// Define Order structure
struct Order {
    string customerName;
    vector<Book> books;
};

// Define User structure
struct User {
    string username;
    string password;
};

bool exitProgram = false;

// Function prototypes
void displayMenu();
void manageAdmin(vector<Book>& books, vector<Order>& orders);
void manageGuest(vector<Order>& orders, vector<Book>& books);
void registerUser(vector<User>& users);
bool loginUser(vector<User>& users);
void logoutUser();
void addBook(vector<Book>& books);
void saveBookDetails(const vector<Book>& books);
void viewBook(vector<Book>& books);
void updateBook(vector<Book>& books);
void loadBookDetails(vector<Book>& books);
void saveBookDetails(const vector<Book>& books);
void viewBookOrders(const vector<Order>& orders);
double calculateTotalPrice(const Order& order, int& discountPercentage);
void addQuotations(vector<Book>& books, vector<Order>& orders );
void printQuotation(const Order& order, int discountPercentage);
void printOrders(const vector<Order>& orders);
void clearConsole();
void exitProgramFunction();


int main() {
    vector<Book> books;
    vector<Order> orders;
    vector<User> users;

    ifstream usersFileIn("users.txt");
    string username, password;
    if (usersFileIn.is_open()) {
       while (usersFileIn >> username >> password) {
           users.push_back({username, password});
    }
           usersFileIn.close();
    } else {
    cerr << "Unable to open users.txt for reading." << endl;
    }

    int loginChoice;
    cout << "______________________________________________________" << endl;
    cout << "============= Welcome to Nethra Bookshop =============" << endl;
    cout << "______________________________________________________" << endl;
    cout << "\n\n\tYou are welcome to Nethra Bookshop ! \n"<< endl;

    do {
        cout << "  1. Login" << endl;
        cout << "  2. Register" << endl;
        cout << "  Enter your choice(1 or 2): ";
        cin >> loginChoice;

        if (loginChoice == 1) {
            clearConsole();
            if (!loginUser(users)) {
                main();

            }
        } else if (loginChoice == 2) {
            clearConsole();
            registerUser(users);
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (loginChoice != 1); // Continue until the user successfully logs in

    int choice;
    do {
        //clearConsole();
        displayMenu();  // Pass admin status to displayMenu()
        cout << "  Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                clearConsole();
                manageAdmin(books, orders);
                break;
            case 2:
                clearConsole();
                manageGuest(orders, books);
                break;
            case 3:
                clearConsole();
                cout << "  Exiting the program. Goodbye!" << endl;
                return 0;
                break;
            default:
                cout << "  Invalid choice. Please try again." << endl;
        }
    } while (choice == 3);

    return 0;
}



// ... (rest of the functions remain the same)

bool loginUser(vector<User>& users) {
    string username, password;
    cout << "\n===================== User Login ====================\n" << endl;
    cout << "  Enter username: ";
    cin >> username;
    cout << "  Enter password: ";
    cin >> password;

    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            if (username.find("admin") == 0) {
                cout << "\n  Admin login successful. Welcome, " << username << "!" << endl;
            } else {
                cout << "\n  Login successful. Welcome, " << username << "!" << endl;
            }
            return true;
        }
    }

    cout << "\n  Login failed. Invalid username or password." << endl;
    return false;
}

void registerUser(vector<User>& users) {
    string username, password;
    cout << "\n================= User Registration =================\n" << endl;
    cout << "\n  Enter desired username: ";
    cin >> username;

    // Check if the username is already taken
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "\n  Username already taken. Please choose a different one." << endl;
            return;
        }
    }

    cout << "\n  Enter password: ";
    cin >> password;

    // Add the new user to the vector
    users.push_back({username, password});
    cout << "\n  Registration successful. Welcome, " << username << "!" << endl;

    ofstream usersFileOut("users.txt");
    if (usersFileOut.is_open()) {
        for (const auto& user : users) {
            usersFileOut << user.username << " " << user.password << endl;
        }
        usersFileOut.close();
    } else {
        cerr << "\n  Unable to open users.txt for writing." << endl;
    }
}

void logoutUser() {
    cout << "\n  Logout successful." << endl;
}

void displayMenu() {
    cout << "\n================== Nethra Bookshop ===================\n" << endl;
    cout << "  Choose your position.\n"<<endl;
    cout << " 1. Admin" << endl;
    cout << " 2. Guest" << endl;
    cout << " 3. Exit" << endl;
}

void addBook(vector<Book>& books) {
    Book newBook;
    cout << "\n============== Add Book ==============\n" << endl;
    cout << "  Enter title: ";
    cin.ignore();
    getline(cin, newBook.title);

    // Check if the book already exists in the system
    auto existingBook = find_if(books.begin(), books.end(),
            [&newBook](const Book& existing) {
                    return existing.title == newBook.title;
            });

    if (existingBook != books.end()) {
            cout << "  Book with the same title already exists in the system. Cannot add the same book again.\n" << endl;
    } else {
            cout << "  Enter author: ";
            getline(cin, newBook.author);
            cout << "  Enter price: Rs.";
            cin >> newBook.price;

            books.push_back(newBook);
            cout << "\n  Book added successfully!" << endl;

            ofstream outFile("bookdetails.txt");
            if (outFile.is_open()) {
                for (const auto& book : books) {
                        outFile << book.title << "\n" << book.author << "\n" << book.price << endl;
                }
                outFile.close();
                cout << "\n  Book details saved to bookdetails.txt" << endl;
            } else {
                cerr << "Unable to open bookdetails.txt for writing." << endl;
            }
        }
}

void loadBookDetails(vector<Book>& books) {
    ifstream inFile("bookdetails.txt");
    if (inFile.is_open()) {
        while (inFile >> ws && !inFile.eof()) {
            Book book;
            getline(inFile, book.title);
            getline(inFile, book.author);
            inFile >> book.price;
            books.push_back(book);
        }
        inFile.close();
        cout << "\n  Book details loaded from bookdetails.txt" << endl;
    } else {
        cerr << "  Unable to open bookdetails.txt for reading." << endl;
    }
}

void viewBook(vector<Book>& books) {
    cout << "\n============= View Books =============\n" << endl;
    for (const auto& book : books) {
            cout << "Title: " << book.title << "\tAuthor: " << book.author << "\tPrice: Rs." << book.price << endl;
    }
}

void searchBook(vector<Book>& books) {
    string searchTerm;
    cout << "\n============ Search Books ============\n" << endl;
    cout << "  Enter search term: ";
    cin.ignore();
    getline(cin, searchTerm);

    cout << "\n  Search results:" << endl;
    for (const auto& book : books) {
        if (book.title.find(searchTerm) != string::npos || book.author.find(searchTerm) != string::npos) {
            cout << "Title: " << book.title << "\tAuthor: " << book.author << "\tPrice: Rs." << book.price << endl;
        }
    }
}

void saveBookDetails(const vector<Book>& books) {
    ofstream outFile("bookdetails.txt");
    if (outFile.is_open()) {
        for (const auto& book : books) {
            outFile << book.title << "\n" << book.author << "\n" << book.price << endl;
        }
        outFile.close();
        cout << "\n  Book details saved to bookdetails.txt" << endl;
    } else {
        cerr << " Unable to open bookdetails.txt for writing." << endl;
    }
}

void updateBook(vector<Book>& books) {
    cout << "\n==================== Update Book =====================\n" << endl;

    // Display available books
    cout << "\n  Select a book to update:" << endl;
    for (size_t i = 0; i < books.size(); ++i) {
        cout << i + 1 << ". " << books[i].title << " by " << books[i].author << "\tPrice: Rs." << books[i].price << endl;
    }

    int bookChoice;
    cout << "\n  Enter the number of the book to update: ";
    cin >> bookChoice;

    if (bookChoice > 0 && bookChoice <= static_cast<int>(books.size())) {
        Book& selectedBook = books[bookChoice - 1];

        // Allow user to update specific details
        cout << "\n  Update details for book: \n" << selectedBook.title << endl;
        cout << "  1. Update Title" << endl;
        cout << "  2. Update Author" << endl;
        cout << "  3. Update Price" << endl;
        cout << "  Enter your choice: \n";

        int updateChoice;
        cin >> updateChoice;

        switch (updateChoice) {
            case 1:
                cout << "  Enter new title: ";
                cin.ignore();
                getline(cin, selectedBook.title);
                break;
            case 2:
                cout << "  Enter new author: ";
                cin.ignore();
                getline(cin, selectedBook.author);
                break;
            case 3:
                cout << "  Enter new price: Rs.";
                cin >> selectedBook.price;
                break;
            default:
                cout << "\n  Invalid choice. No updates applied." << endl;
                return;
        }

        cout << "\n  Book details updated successfully!" << endl;

        // Save updated book details to "bookdetails.txt"
        saveBookDetails(books);
    } else {
        cout << "  Invalid book choice. No updates applied." << endl;
    }
}

void deleteBook(vector<Book>& books){{
    cout << "\n============= Delete Book ============\n" << endl;
    string deleteTitle;
    cout << "  Enter the title of the book to delete: ";
    cin.ignore();
    getline(cin, deleteTitle);

    auto bookToDelete = find_if(books.begin(), books.end(), [&deleteTitle](const Book& book) {
            return book.title == deleteTitle;
    });

    if (bookToDelete != books.end()) {
        books.erase(bookToDelete);
        cout << "\n  Book deleted successfully!" << endl;

        // Save the updated book details to "bookdetails.txt"
        saveBookDetails(books);
    } else {
        cout << "\n  Book not found. Please check the title and try again." << endl;
    }
  }
}

void viewBookOrders(const vector<Order>& orders) {
    ifstream orderFileIn("order.txt");
    if (orderFileIn.is_open()) {
        string line;
        cout << "\n================== View Book Orders ==================" << endl;

        while (getline(orderFileIn, line)) {
            cout << line << endl;
        }

        orderFileIn.close();
    } else {
        cerr << "  Unable to open order.txt for reading." << endl;
    }
}

void addQuotations(vector<Book>& books, vector<Order>& orders ) {
       Order newOrder;
       cout << "\n========== Prepare Quotations =========\n" << endl;
       cout << "  Enter customer name: ";
       cin.ignore();
       getline(cin, newOrder.customerName);

       int bookChoice;
       int totalPrice1 = 0;
   do {
       // Display available books
       cout << "\n  Select a book to add to the order (0 to finish):" << endl;
       for (size_t i = 0; i < books.size(); ++i) {
           cout << i + 1 << ". " << books[i].title << " by " << books[i].author << "\tPrice: Rs." << books[i].price << endl;
       }
       cin >> bookChoice;

       if (bookChoice > 0 && bookChoice <= static_cast<int>(books.size())) {
           int quantity;
           cout << "\n  Enter quantity for the selected book: ";
           cin >> quantity;

           if (quantity > 0) {
               // Update the quantity field for the selected book
               books[bookChoice - 1].quantity = quantity;

               // Add the book with quantity to the order
               newOrder.books.push_back(books[bookChoice - 1]);
               cout << "\n  Book added to the order." << endl;
           } else {
               cout << "\n  Invalid quantity. Please enter a quantity greater than 0." << endl;
           }
       } else if (bookChoice != 0) {
           cout << "\n  Invalid choice. Please try again." << endl;
       }
   } while (bookChoice != 0);

   orders.push_back(newOrder);
   cout << "\n  Quotations prepared successfully!" << endl;

   // Display added item quantities
   cout << "\n  Added Item Quantities:" << endl;
   for (const auto& book : newOrder.books) {
           cout << "Title: " << book.title << "\tAuthor: " << book.author << "\tPrice: Rs." << book.price << "\tQuantity: " << book.quantity << endl;
           totalPrice1 += book.price * book.quantity;
   }

   int discountPercentage = 0;
   double discountedPrice = calculateTotalPrice(newOrder, discountPercentage);

   cout << "\n  Total Price before Discount: Rs." << totalPrice1 << endl;
   cout << "  Discount Percentage: " << discountPercentage << "%" << endl;
   cout << "  Discount total Price: Rs." << discountedPrice << endl;

   // Save order details to "order.txt"
   ofstream orderFileOut("order.txt", ios::app);
   if (orderFileOut.is_open()) {
       orderFileOut << "Customer: " << newOrder.customerName << endl;
       orderFileOut << "Added Item Quantities:" << endl;
       for (const auto& book : newOrder.books) {
               orderFileOut << "Title: " << book.title << "\tAuthor: " << book.author << "\tPrice: Rs." << book.price
                        << "\tQuantity: " << book.quantity << endl;
       }
       orderFileOut << "Total Price before Discount: Rs." << totalPrice1 << endl;
       orderFileOut << "Discount Percentage: " << discountPercentage << "%" << endl;
       orderFileOut << "Discounted Price: Rs." << discountedPrice << endl;
       orderFileOut << "-----------------------------" << endl;
       orderFileOut.close();
       cout << "\n  Order details saved to order.txt" << endl;
   } else {
       cerr << "  Unable to open order.txt for writing." << endl;
   }
}

void printQuotation(const Order& order, int discountPercentage) {
    cout << "\n==================== Quotation ======================\n" << endl;
    cout << "Customer: " << order.customerName << endl;
    cout << "-----------------------------" << endl;

    // Read and print the quotation details from "order.txt"
    ifstream orderFileIn("order.txt");
    if (orderFileIn.is_open()) {
        string line;
        while (getline(orderFileIn, line)) {
            if (line.find("Customer: " + order.customerName) != string::npos) {
                cout << line << endl;  // Print the customer name line
                getline(orderFileIn, line);  // Skip the "Added Item Quantities:" line

                // Print the added item quantities
                while (getline(orderFileIn, line) && line != "-----------------------------") {
                    cout << line << endl;
                }

                cout << "-----------------------------" << endl;
                break;
            }
        }

        orderFileIn.close();
    } else {
        cerr << "  Unable to open order.txt for reading." << endl;
    }
}

void printOrders(const vector<Order>& orders) {
    ifstream orderFileIn("order.txt");
    if (orderFileIn.is_open()) {
        string orderLine;
        cout << "\n================== View Book Orders ==================" << endl;

        // Get the customer name for filtering
        string customerName;
        cout << "Enter customer name to view orders: ";
        cin.ignore();
        getline(cin, customerName);

        // Iterate through the file to find orders for the specified customer
        while (getline(orderFileIn, orderLine)) {
            if (orderLine.find("Customer: " + customerName) != string::npos) {
                // Parse the order details and print only the customer name
                Order order;
                order.customerName = customerName;
                printQuotation(order, 0); // Assuming 0 discount for simplicity
            }
        }

        orderFileIn.close();
    } else {
        cerr << "  Unable to open order.txt for reading." << endl;
    }
}

void manageAdmin(vector<Book>& books, vector<Order>& orders ) {
    loadBookDetails(books);
    int choice;
    do {
        cout << "\n==================== Manage Books ====================\n" << endl;
        cout << "  1. View Books" << endl;
        cout << "  2. Add Book" << endl;
        cout << "  3. Update Book" << endl;
        cout << "  4. Delete Book" << endl;
        cout << "  5. Search Books" << endl;
        cout << "\n==================== Manage Orders ===================\n" << endl;
        cout << "  6. View Book Quotations" << endl;
        cout << "  7. Add Quotations" << endl;
        cout << "  8. Print Quotations" << endl;
        cout << "  9. Logout" << endl;
        cout << "  10. Exit" << endl;
        cout << "  Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                clearConsole();
                viewBook(books);
                break;
            case 2:
                clearConsole();
                addBook(books);
                break;
             case 3:
                clearConsole();
                updateBook(books);
                break;
             case 4:
                clearConsole();
                deleteBook(books);
                break;
            case 5:
                clearConsole();
                searchBook(books);
                break;
            case 6:
                clearConsole();
                viewBookOrders(orders);
                break;
            case 7:
                clearConsole();
                addQuotations(books, orders );
                break;

             case 8:
                clearConsole();
                printOrders(orders);
                break;
            case 9:
                clearConsole();
                cout << "\n  User logout. Need to login again." << endl;
                main();
                break;
            case 10:
                clearConsole();
                exitProgramFunction();
                break;
            default:
                clearConsole();
                cout << "\n  Invalid choice. Please try again." << endl;
        }
    } while (choice != 10);

}

void manageGuest(vector<Order>& orders, vector<Book>& books) {
    loadBookDetails(books);
    int choice;
    do {
        cout << "\n==================== Manage Books ====================\n" << endl;
        cout << "  1. View Books" << endl;
        cout << "  2. Search Books" << endl;
        cout << "  3. Add Book" << endl;
        cout << "\n==================== Manage Orders ===================\n" << endl;
        cout << "  4. Add quotations" << endl;
        cout << "  5. Print Quotations" << endl;
        cout << "  6. Logout" << endl;
        cout << "  7. Exit" << endl;
        cout << "  Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                clearConsole();
                viewBook(books);
                break;
             case 2:
                clearConsole();
                searchBook(books);
                break;
            case 3:
                clearConsole();
                addBook(books);
                break;
            case 4:
                clearConsole();
                addQuotations(books, orders );
                break;
            case 5:
                clearConsole();
                printOrders(orders);
                break;
            case 6:
                clearConsole();
                cout << "\n  User logout. Need to login again." << endl;
                main();
                break;
            case 7:
                clearConsole();
                exitProgramFunction();
                break;
            default:
                clearConsole();
                cout <<""<<choice<<"  Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);

}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void exitProgramFunction() {
    exitProgram = true;
    cout << "\n  Exiting the program. Goodbye!" << endl;
}


double calculateTotalPrice(const Order& order, int& discountPercentage) {
    double totalPrice = 0.0;

    for (const auto& book : order.books) {
        totalPrice += book.price * book.quantity;

    }

    // Apply discount based on the total quantity of books
    int totalQuantity = 0;
    for (const auto& book : order.books) {
        totalQuantity += book.quantity;
    }

    if (totalQuantity >= 1 && totalQuantity < 5) {
        // Apply a 10% discount for quantity less than 5
        discountPercentage = 10;
        totalPrice *= 0.9;
    } else if (totalQuantity >= 5) {
        // Apply a 15% discount for quantity 5 or more
        discountPercentage = 15;
        totalPrice *= 0.85;
    } else {
        discountPercentage = 0;
    }

    return totalPrice;
}

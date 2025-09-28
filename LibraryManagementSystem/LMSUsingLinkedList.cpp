#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

// Time Complexity:O(n+m)
// Space Complexity:O(1)

struct Book
{
    string title;
    string author;
    bool isCheckout;
};

struct HashNode
{
    int val;
    Book books;
    HashNode *next;
};

class HashTable
{
private:
    vector<HashNode *> table;
    int table_size;

    int hashFunction(int key)
    {
        return (key*31) % table_size;
    }

public:
    HashTable(int size = 100)
    {
        table_size = size;
        table.resize(table_size, nullptr);
    }

    ~HashTable()
    {
        for (int i = 0; i < table_size; ++i)
        {
            HashNode *head = table[i];
            while (head != nullptr)
            {
                HashNode *temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

    // CRUD
    void addBooks()
    {
        int bookId;
        cout << "Enter The Id: " << endl;
        cin >> bookId;

        int temp_index = hashFunction(bookId);
        HashNode* temp_head = table[temp_index];
        while (temp_head != nullptr) {
            if (temp_head->val == bookId) {
                cout << "Error: A book with this ID already exists.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            temp_head = temp_head->next;
        }

        string title;
        cout << "Enter The Title Of The Book: " << endl;
        cin.ignore();
        getline(cin, title);

        string author;
        cout << "Enter The Author Of The Book: " << endl;
        getline(cin, author);

        int index = hashFunction(bookId);

        HashNode *newBook = new HashNode();
        newBook->val = bookId;
        newBook->books = {title, author, false};
        newBook->next = nullptr;

        if (table[index] == nullptr)
        {
            table[index] = newBook;
        }
        else
        {
            newBook->next = table[index];
            table[index] = newBook;
        }
        cout << "Book added successfully!\n";
    }
    void findBooks()
    {
        int bookId;
        cout << "Enter The Id: " << endl;
        cin >> bookId;

        int index = hashFunction(bookId);
        HashNode *head = table[index];

        while (head != nullptr)
        {
            if (head->val == bookId)
            {
                cout << "--- Book Found ---\n";
                cout << "Title: " << head->books.title << endl;
                cout << "Author: " << head->books.author << endl;
                cout << "Checkout Status: " << (head->books.isCheckout ? "Yes" : "No") << endl;
                return;
            }
            head = head->next;
        }
        cout << "Error: Book with ID " << bookId << " not found.\n";
    }
    void updateBooks()
    {
        int bookId;
        cout << "Enter The Id: " << endl;
        cin >> bookId;

        int ch;
        cout << "1. Change The Author Name Of The Book" << endl;
        cout << "2. Change The Title Of The Book" << endl;
        cout << "3. Change The Checkout Status Of The Book" << endl;
        cout << "Enter your Choice: " << endl;
        cin >> ch;

        int index = hashFunction(bookId);
        HashNode *head = table[index];

        while (head != nullptr)
        {
            if (head->val == bookId)
            {
                if (ch == 1)
                {
                    string newAuthor;
                    cout << "Enter The New Name Of The Author: " << endl;
                    cin.ignore();
                    getline(cin, newAuthor);

                    head->books.author = newAuthor;
                    cout << "Success!!" << endl;
                    return;
                }
                else if (ch == 2)
                {
                    string newTitle;
                    cout << "Enter The New Title Of The Author: " << endl;
                    cin.ignore();
                    getline(cin, newTitle);

                    head->books.title = newTitle;
                    cout << "Success!!" << endl;
                    return;
                }
                else if (ch == 3)
                {
                    head->books.isCheckout = !head->books.isCheckout;
                    cout << "Success!!" << endl;
                    return;
                }
                else
                {
                    cout << "Invalid choice. Returning to the main menu." << endl;
                }
                return;
            }
            head = head->next;
        }
    }
    void removeBooks()
    {
        int bookId;
        cout << "Enter The Id: " << endl;
        cin >> bookId;

        int index = hashFunction(bookId);
        HashNode *curr=table[index];
        HashNode *prev=nullptr;

        while (curr != nullptr && curr->val != bookId)
        {
            prev = curr;
            curr = curr->next;
        }

        if(curr == nullptr)
        {
            cout<<"Library Is Empty"<<endl;
            return;
        }

        if (prev == nullptr)
        {
            table[index] = curr->next;
        }
        else
        {
            prev->next = curr->next;
        }

        delete curr;
        cout << "Book removed successfully!\n";
    }

    // File I/O
    void saveLibraryToFile(const string &filename)
    {
        ofstream outputFile(filename);
        if (!outputFile.is_open())
        {
            cout << "Error: Could not save to file.\n";
            return;
        }

        for (int i = 0; i < table_size; ++i)
        {
            HashNode *head = table[i];
            while (head != nullptr)
            {
                outputFile << head->val << endl;
                outputFile << head->books.title << endl;
                outputFile << head->books.author << endl;
                outputFile << head->books.isCheckout << endl;
                head=head->next;
            }
        }

        cout << "Library data saved successfully!\n";
        outputFile.close();
    }

    void loadLibraryFromFile(const string &filename)
    {
        ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            cout << "No existing library file. Starting a new library.\n";
            return;
        }

        int id;
        string title, author;
        bool isCheckout;

        while (inputFile >> id)
        {
            inputFile.ignore();

            getline(inputFile, title);
            getline(inputFile, author);
            inputFile >> isCheckout;

            Book loadedBook = {title, author, isCheckout};

            int index = hashFunction(id);

            HashNode *newNode = new HashNode();
            newNode->val = id;
            newNode->books = loadedBook;

            newNode->next = table[index];
            table[index] = newNode;
        }

        cout << "Library data loaded successfully!\n";
        inputFile.close();
    }

    // Menu
    void librarySystem()
    {

        string filename;
        cout << "Enter The File Name You Want To Read/Write To (e.g., library.txt): ";
        getline(cin, filename);
        loadLibraryFromFile(filename);


        while (true)
        {
            cout << "\n--- Library Menu (Custom Hash Table) ---\n";
            cout << "1. Add A Book\n";
            cout << "2. Find A Book\n";
            cout << "3. Update A Book\n";
            cout << "4. Remove A Book\n";
            cout << "Enter The Choice: ";

            int ch;
            cin >> ch;

            if (cin.fail())
            {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (ch)
            {
            case 1:
                addBooks();
                break;
            case 2:
                findBooks();
                break;
            case 3:
                updateBooks();
                break;
            case 4:
                removeBooks();
                break;
            default:
                saveLibraryToFile(filename);
                return;
            }
        }
    }
};

int main()
{
    HashTable myLibrary;
    myLibrary.librarySystem();
    return 0;
}
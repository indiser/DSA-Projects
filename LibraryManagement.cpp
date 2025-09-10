#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include<fstream>
#include<sstream>
using namespace std;

struct Book
{
    string title;
    string author;
    bool isCheckout;
};

class Solution
{
private:
    unordered_map<int, Book> mp;
    void addBooks()
    {
        int val;
        cout << "Enter the index number of the Book: " << endl;
        cin >> val;

        string title;
        cout << "Enter The Title Of The Book: " << endl;
        cin.ignore();
        getline(cin, title);

        string author;
        cout << "Enter The Author Of The Book: " << endl;
        getline(cin, author);

        Book newBook;
        newBook.author = author;
        newBook.title = title;
        newBook.isCheckout = false;

        mp[val] = newBook;
    }
    void findBooks()
    {
        if (mp.empty())
        {
            cout << "The Library is Empty" << endl;
            return;
        }
        int bookId;
        cout << "Enter The Book Id: " << endl;
        cin >> bookId;

        if (mp.find(bookId) != mp.end())
        {
            cout << "Title Of The Book: " << mp[bookId].title << endl;
            cout << "Author Of The Book: " << mp[bookId].author << endl;
            cout << "Check Out Status: " << (mp[bookId].isCheckout ? "Yes" : "No") << endl;
        }
        else
        {
            cout << "Error: Book with this ID not found in the library." << endl;
        }
    }
    void updateBooks()
    {
        if (mp.empty())
        {
            cout << "The Library is Empty" << endl;
            return;
        }
        int bookId;
        cout << "Enter The Book Id: " << endl;
        cin >> bookId;

        if (mp.find(bookId) != mp.end())
        {
            int ch;
            cout << "1. Change The Author Name Of The Book" << endl;
            cout << "2. Change The Title Of The Book" << endl;
            cout << "3. Change The Checkout Status Of The Book" << endl;
            cout << "Enter your Choice: " << endl;
            cin >> ch;

            if(ch==1)
            {
                string newAuthor;
                cout << "Enter The New Name Of The Author: " << endl;
                cin.ignore();
                getline(cin, newAuthor);
                mp[bookId].author = newAuthor;
                cout << "Success!!" << endl;
            }
            else if(ch==2)
            {
                string newTitle;
                cout << "Enter The New Title Of The Author: " << endl;
                cin.ignore();
                getline(cin, newTitle);
                mp[bookId].title = newTitle;
                cout << "Success!!" << endl;
            }
            else if(ch==3)
            {
                mp[bookId].isCheckout = !mp[bookId].isCheckout;
                cout<<"Success!!"<<endl;
            }
            else
            {
                cout << "Invalid choice. Returning to the main menu." << endl;
            }
        }
        else
        {
            cout << "Error: Book with this ID not found in the library." << endl;
        }
    }
    void removeBooks()
    {
        if (mp.empty())
        {
            cout << "The Library is Empty" << endl;
            return;
        }
        int bookId;
        cout << "Enter The Book Id: " << endl;
        cin >> bookId;

        if (mp.find(bookId) != mp.end())
        {
            mp.erase(bookId);
            cout<<"Success!!"<<endl;
        }
        else
        {
            cout << "Error: Book with this ID not found in the library." << endl;
        }
    }

    void loadLibraryFromFile(const string& filename)
    {
        ifstream inputFile(filename);
        if(!inputFile.is_open())
        {
            cout << "No existing library file. Starting a new library." << endl;
            return;
        }

        int id;
        string author,title;
        bool isCheckedOut;

        while(inputFile >> id)
        {
            inputFile.ignore();
            getline(inputFile,title);
            getline(inputFile,author);

            inputFile >> isCheckedOut;

            Book newBook={title,author,isCheckedOut};
            mp[id]=newBook;
        }

        cout << "Library data loaded successfully!" << endl;
        inputFile.close();
    }
    void saveLibraryToFile(const string& filename)
    {
        ofstream outputFile(filename);

        if (!outputFile.is_open())
        {
            cout << "Error: Could not save to file." << endl;
            return;
        }

        for(auto const& pair:mp)
        {
            int bookId=pair.first;
            const Book& book=pair.second;

            outputFile << bookId << endl;
            outputFile << book.title << endl;
            outputFile << book.author << endl;
            outputFile << book.isCheckout << endl;
        }

        cout << "Library data saved successfully!" << endl;
        outputFile.close();
    }

public:
    void librarySystem()
    {
        string filename;
        cout<<"Enter The File Name You Want To  Read From: "<<endl;
        getline(cin,filename);
        loadLibraryFromFile(filename);
        while (true)
        {
            int ch;
            cout << "1.Add A Book" << endl;
            cout << "2.Find A Book" << endl;
            cout << "3. Update A Book" << endl;
            cout << "4. Remove A Book" << endl;
            cout << "Enter The Choice: " << endl;
            cin >> ch;

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
    Solution s;
    s.librarySystem();
}
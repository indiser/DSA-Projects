#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <Windows.h>
using namespace std;

// Time complexity:O(n^2)
// Space Complexity:O(n)

struct Students
{
    string name_of_student;
    float marks_of_student;
    Students(string name, float marks)
    {
        name_of_student = name;
        marks_of_student = marks;
    }
};

class Solution
{
private:
    vector<Students> records;
    void printHeader(const string &title)
    {
        cout << "\n\n"; // Add some space before
        cout << "========================================\n";
        cout << "        " << title << "        \n";
        cout << "========================================\n";
    }
    void createRecord()
    {
        string name;
        cout << "Enter The Name Of The Student: " << endl;
        cin.ignore();
        getline(cin, name);

        float marks;
        cout << "Enter The Marks Of Student: " << endl;
        cin >> marks;

        records.emplace_back(name, marks);
    }
    void displayRecords()
    {
        if (records.empty())
        {
            cout << "Nothing To Show Yet!!" << endl;
            return;
        }

        printHeader("Student Records");
        cout << left << setw(25) << "Student Name" << " | " << "Marks" << endl;
        cout << "----------------------------------------\n";
        for (auto const &student : records)
        {
            cout << left << setw(25) << student.name_of_student << " | "
                 << fixed << setprecision(2) << student.marks_of_student << endl;
        }
        cout << "========================================\n\n";
    }
    void searchRecords()
    {
        if (records.empty())
        {
            cout << "Nothing To Show Yet!!" << endl;
            return;
        }

        string searchName;
        cout << "Enter The Student Name: " << endl;
        cin.ignore();
        getline(cin, searchName);
        bool found = false;

        for (auto const &student : records)
        {
            if (student.name_of_student == searchName)
            {
                cout << "Name: " << student.name_of_student << "||" << "Marks: " << student.marks_of_student << endl;
                found = true;
            }
        }

        if (found == false)
        {
            cout << "Stduent Records Not Found" << endl;
        }
    }
    void updateRecords()
    {
        if (records.empty())
        {
            cout << "Nothing To Show Yet!!" << endl;
            return;
        }

        string name;
        cout << "Enter The Name Of The Student" << endl;
        cin.ignore();
        getline(cin, name);

        bool found = false;

        for (auto &student : records)
        {
            if (student.name_of_student == name)
            {
                float marks;
                cout << "Enter The New Marks: " << endl;
                cin >> marks;

                student.marks_of_student = marks;
                cout << "Success!" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Student Records Not Found" << endl;
        }
    }
    void deleteRecords()
    {
        if (records.empty())
        {
            cout << "Nothing To Show Yet!!" << endl;
            return;
        }

        string name;
        cout << "Enter The Name Of The Student: " << endl;
        cin.ignore();
        getline(cin, name);

        bool found = false;
        for (int i = 0, len = records.size(); i < len; i++)
        {
            if (records[i].name_of_student == name)
            {
                records.erase(records.begin() + i);
                cout << "Success!!" << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Student Records Not Found" << endl;
        }
    }
    void sortRecords()
    {
        if (records.empty())
        {
            cout << "Nothing To Show Yet!!" << endl;
            return;
        }

        for (int i = 0, len = records.size(); i < len; i++)
        {
            for (int j = 0; j < len - i - 1; j++)
            {
                if (records[j].marks_of_student > records[j + 1].marks_of_student)
                {
                    Students temp = records[j];
                    records[j] = records[j + 1];
                    records[j + 1] = temp;
                }
            }
        }

        cout << "Records Sorted By Marks: " << endl;
        displayRecords();
    }

public:
    void Records()
    {
        while (true)
        {
            int ch;
            cout<<"`````````````````````````````````````````"<<endl;
            cout << "1. Create A New Strudent Record" << endl;
            cout << "2. Display All Student Records" << endl;
            cout << "3. Search For A Students Record" << endl;
            cout << "4. Update A Students Record" << endl;
            cout << "5. Delete A Students Record" << endl;
            cout << "6. Sort All The Records" << endl;
            cout<<"`````````````````````````````````````````"<<endl;
            cout << "Enter A Choice: " << endl;
            cin >> ch;
            switch (ch)
            {
            case 1:
                createRecord();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
                searchRecords();
                break;
            case 4:
                updateRecords();
                break;
            case 5:
                deleteRecords();
                break;
            case 6:
                sortRecords();
                break;
            default:
                return;
            }
        }
    }
};

int main()
{
    Solution s;
    s.Records();
}
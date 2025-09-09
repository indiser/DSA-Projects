#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

class Solution
{
private:
    stack<float> history;
    float result;
    float addition(int a, int b)
    {
        return a + b;
    }
    float substraction(int a, int b)
    {
        return a - b;
    }
    float multiplication(int a, int b)
    {
        return a * b;
    }
    float division(int a, int b)
    {
        
        return a / b;
    }

public:
    
    void calculations()
    {

        while (true)
        {
            cout << "Press 1 for addition" << endl;
            cout << "Press 2 for substraction" << endl;
            cout << "Press 3 for multiplication" << endl;
            cout << "Press 4 for division" << endl;
            cout << "Press 0 to exit" << endl;
            int ch;
            cout << "Enter Your Choice" << endl;
            cin >> ch;
            if(ch==0)
            {
                return;
            }
            float a;
            cout << "Enter The First Number:" << endl;
            cin >> a;
            float b;
            cout << "Enter The Second Number:" << endl;
            cin >> b;
            switch (ch)
            {
            case 1:
                result = addition(a, b);
                cout<<"Result :"<<result<<endl;
                history.push(result);
                break;
            case 2:
                result = substraction(a, b);
                cout<<"Result :"<<result<<endl;
                history.push(result);
                break;
            case 3:
                result = multiplication(a, b);
                cout<<"Result :"<<result<<endl;
                history.push(result);
                break;
            case 4:
                if(b==0)
                {
                    cout<<"Error dividing by zero"<<endl;
                    continue;
                }
                result = division(a, b);
                cout<<"Result :"<<result<<endl;
                history.push(result);
                break;
            default:
                return;
            }
        }
    }
    void showHistory()
    {
        cout << "The Recent History is:" << endl;
        while (!history.empty())
        {
            cout << history.top() << endl;
            history.pop();
        }
    }
};

int main()
{
    Solution s;
    s.calculations();
    s.showHistory();
}
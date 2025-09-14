#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <Windows.h>
using namespace std;



class Solution
{
private:
    const vector<int> OriginalArr = {10, 9, 8, 7, 6, 5, 4, 3};
    vector<int> arr;

    void swap(int &a, int &b)
    {
        int temp=a;
        a=b;
        b=temp;
    }
    void merge(vector<int> &arr, int start, int mid, int end)
    {
        int i = start, j = mid + 1;
        vector<int> temp;
        while (i <= mid && j <= end)
        {
            if (arr[i] <= arr[j])
            {
                temp.push_back(arr[i]);
                i++;
            }
            else
            {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid)
        {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= end)
        {
            temp.push_back(arr[j]);
            j++;
        }

        for (int index = 0, len = temp.size(); index < len; index++)
        {
            arr[start + index] = temp[index];
        }
        displayArray(arr);
        Sleep(50);
    }

    void mergeSort(vector<int> &arr, int start, int end)
    {
        if (start < end)
        {
            int mid = start + (end - start) / 2;
            mergeSort(arr, start, mid);
            mergeSort(arr, mid + 1, end);
            merge(arr, start, mid, end);
        }
    }

    int partition(vector<int> &arr, int start, int end)
    {
        int pivot=arr[end];
        int i=start-1;

        for (int j = start; j < end; j++)
        {
            if(arr[j] < pivot)
            {
                i++;
                swap(arr[i],arr[j]);
                displayArray(arr);
                Sleep(50);
            }
        }
        
        swap(arr[i+1],arr[end]);
        displayArray(arr);
        Sleep(50);
        return i+1;
    }

    void quickSort(vector<int> &arr, int start, int end)
    {
        if(start < end)
        {
            int pi=partition(arr,start,end);
            quickSort(arr,start,pi-1);
            quickSort(arr,pi+1,end);
        }
    }

    void heapify(vector<int> &arr, int n, int i)
    {
        int largest=i;
        int left=2*i+1;
        int right=2*i+2;

        if(left<n && arr[left] > arr[largest]) largest=left;
        if(right<n && arr[right] > arr[largest]) largest=right;

        if(largest != i)
        {
            swap(arr[i],arr[largest]);
            displayArray(arr);
            Sleep(50);
            heapify(arr,n,largest);
        }
    }
    void heapSort(vector<int> &arr, int n)
    {

        for (int i = n/2-1; i >= 0; i--)
        {
            heapify(arr,n,i);
        }

        for (int i = n-1; i >= 0; i--)
        {
            swap(arr[0],arr[i]);
            heapify(arr,i,0);
            displayArray(arr);
            Sleep(50);
        }
    }
    void clearConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordScreen = {0, 0};
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition(hConsole, coordScreen);
    }

    void displayArray(const vector<int> &arr)
    {
        clearConsole();
        for (int val : arr)
        {
            cout << val << " | ";
            for (int i = 0; i < val; i++)
            {
                cout << "█";
            }
            cout << endl;
        }
    }

    void displaySortedArray()
    {
        for(int val:arr)
        {
            cout<<val<<" ";
        }
    }

public:
    void run()
    {
        while(true)
        {
            int ch;
            cout<<"```````````````````````````````````````"<<endl;
            cout<<"1. Merge Sort"<<endl;
            cout<<"2. Quick Sort"<<endl;
            cout<<"3. Heap Sort"<<endl;
            cout<<"Enter The Choice: "<<endl;
            cout<<"```````````````````````````````````````"<<endl;
            cin>>ch;

            switch (ch)
            {
            case 1:
                arr=OriginalArr;
                mergeSort(arr,0,arr.size()-1);
                cout<<"After Merge Sort"<<endl;
                displaySortedArray();
                cout<<endl;
                break;
            case 2:
                arr=OriginalArr;
                quickSort(arr,0,arr.size()-1);
                cout<<"After Quick Sort"<<endl;
                displaySortedArray();
                cout<<endl;
                break;
            case 3:
                arr=OriginalArr;
                heapSort(arr,arr.size());
                cout<<"After Heap Sort"<<endl;
                displaySortedArray();
                cout<<endl;
                break;
            default:
                cout<<"Invalid Choice!!"<<endl;
                return;
            }
        }
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    Solution s;
    s.run();
    return 0;
}
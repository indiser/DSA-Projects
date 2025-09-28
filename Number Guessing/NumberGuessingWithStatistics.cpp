#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <ctime>
using namespace std;

int lowerBound = 1, upperBound = 10;

class Solution
{
private:
    mt19937 gen;
    int randomNumberGenerator()
    {
        uniform_int_distribution<> distrib(lowerBound, upperBound);
        return distrib(gen);
    }
    void merge(vector<int> &arr, int start, int mid, int end)
    {
        int i = start, j = mid + 1;
        vector<int> temp;
        while (i <= mid && j <= end)
        {
            if (arr[i] < arr[j])
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

    bool binarySearch(vector<int>&arr, int target, int start, int end)
    {
        if(start >  end) return false;
        int mid=start+(end-start)/2;
        if(arr[mid]==target) return true;
        else if(arr[mid] < target) return binarySearch(arr,target,mid+1, end);
        else return binarySearch(arr,target,start, mid-1);

    }

public:

    Solution() : gen(time(nullptr)) {}

    void numberGuesser()
    {
        vector<int> ans;
        int secret_number = randomNumberGenerator();
        int count = 0;
        int guess;

        cout<<"`````````START OF GAME```````````"<<endl;

        cout << "RANDOM NUMBERS ARE GENERATED FROM " <<lowerBound << " TO " << upperBound<< endl;
        while (true)
        {
            cout << "Enter your Guess: " << endl;
            cin >> guess;

            ans.push_back(guess);
            count++;

            if (guess > secret_number)
            {
                cout << "Too High!" << endl;
            }
            else if (guess < secret_number)
            {
                cout << "Too Low!" << endl;
            }
            else
            {
                cout << "You Got It!!!!" << endl;
                break;
            }
        }
        cout<<"````````````````STATISTICS`````````````````"<<endl;
        cout << "The Secret number is: " << secret_number << endl;
        cout << "The Number of tries it took: " << count << endl;
        mergeSort(ans, 0, ans.size() - 1);
        cout<<"The guesses were"<<endl;
        for (int val : ans)
        {
            cout << val << " ";
        }
        cout << endl;

        cout<<"`````````````````QUESTION``````````````````````"<<endl;
        int searchNumber;
        cout<<"Have I Ever Guessed This Number???"<<endl;
        cin>>searchNumber;
        if(binarySearch(ans,searchNumber,0,ans.size()-1))
        {
            cout<<"Yes You Have"<<endl;
        }
        else
        {
            cout<<"No you have not"<<endl;
        }

        cout<<"`````````````END OF GAME``````````````````````"<<endl;
        cout<<endl;

        cout<<"Do YOU WANT TO PLAY AGAIN??? THEN TYPE YES"<<endl;
        string choice;
        cin>>choice;

        if(choice == "YES" || choice =="Y" || choice == "y" || choice =="yes")
        {
            numberGuesser();
        }
        else
        {
            exit(0);
        }
    }
};

int main()
{
    Solution s;
    s.numberGuesser();
    return 0;
}
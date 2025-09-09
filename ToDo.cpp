#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<Windows.h>
using namespace std;

// Time Complexity:O(n)
// Space Complexity:O(1)

struct Task
{
    int id;
    string name_of_the_task;
    bool is_completed=false;
};

struct ListNode
{
    Task data;
    ListNode *next;
    ListNode(string task)
    {
        data.name_of_the_task=task;
        data.is_completed=false;
        next=nullptr;
    }
    
};

class Solution
{
    private:
        int nextTaskId=1;
        ListNode *head=nullptr;
        ListNode *createTask(ListNode *head)
        {
            string input;
            cout << "Enter The Task You Wish To Create: ";
            cin.ignore();
            getline(cin,input);

            ListNode *newNode=new ListNode(input);
            newNode->data.id=nextTaskId;
            nextTaskId++;
            if(head==nullptr) return newNode;
            ListNode *temp=head;
            while(temp->next != nullptr)
            {
                temp=temp->next;
            }
            cout<<"Success!"<<endl;
            temp->next=newNode;
            return head;

        }
        void readTask(ListNode *head)
        {
            if(head == nullptr)
            {
                cout<<"No Tasks Yet!!"<<endl;
                return;
            }
            cout << "Name Of The Tasks:" << endl;
            while(head != NULL)
            {
                cout << head->data.id << ". [" << (head->data.is_completed ? "✓" : "x") << "] " << head->data.name_of_the_task << endl;
                head = head->next;
            }
        }
        ListNode *changeStatus(ListNode *head)
        {
            if(head == nullptr)
            {
                cout<<"No Tasks Yet!!"<<endl;
                return head;
            }

            int uniqueId;
            cout<<"Enter The Unique Id Of The Task You Want To Change The Status Of: "<<endl;
            cin>>uniqueId;

            ListNode *temp=head;

            while(temp != NULL)
            {
                if(uniqueId==temp->data.id)
                {
                    temp->data.is_completed=!temp->data.is_completed;
                    cout<<"Success!"<<endl;
                    return head;
                }
                temp=temp->next;
            }
            cout<<"Task Is Not Found"<<endl;
            return head;
        }
        ListNode *updateTaskName(ListNode *head)
        {
            if(head == nullptr)
            {
                cout<<"No Tasks Yet!!"<<endl;
                return head;
            }
            ListNode *temp=head;

            int uniqueId;
            cout<<"Enter The Id Of The Task That You Want To Update: "<<endl;
            cin>>uniqueId;

            while(temp != NULL)
            {
                if(uniqueId==temp->data.id)
                {
                    string newTask;
                    cout<<"Enter The Name Of The New Task"<<endl;
                    cin.ignore();
                    getline(cin,newTask);
                    temp->data.name_of_the_task=newTask;
                    cout<<"Success!"<<endl;
                    return head;
                }
                temp=temp->next;
            }
            cout<<"Task Not Found!"<<endl;
            return head;
        }
        ListNode *deleteTask(ListNode *head)
        {
            if(head == nullptr)
            {
                cout<<"No Task Yet!!"<<endl;
                return head;
            }
            int uniqueId;
            cout<<"Enter The Id Of The Task To Delete"<<endl;
            cin>>uniqueId;

            //To Delete The Head node
            if(uniqueId == head->data.id)
            {
                ListNode *temp=head;
                head=head->next;
                delete temp;
                return head;
            }

            //Delete From Middle Or End
            ListNode *prev=nullptr;
            ListNode *curr=head;

            while(curr != NULL && curr->data.id != uniqueId)
            {
                prev=curr;
                curr=curr->next;
            }

            if(curr != NULL)
            {
                prev->next=curr->next;
                delete curr;
                return head;
            }
            else
            {
                cout<<"Task Was Not Found"<<endl;
            }
            return head;
        }
    public:
        void ToDo()
        {
            while(true)
            {
                int ch;
                cout<<"``````````````````````````````````````````"<<endl;
                cout<<"1. Create a Task "<<endl;
                cout<<"2. Traverse Or Display The Tasks "<<endl;
                cout<<"3. Update The List Of Tasks "<<endl;
                cout<<"4. Change The Status Of A Task"<<endl;
                cout<<"5. Delete A Task"<<endl;
                cout<<"``````````````````````````````````````````"<<endl;
                cout<<"Enter Your Choice: "<<endl;
                cin>>ch;
                switch (ch)
                {
                case 1:
                    head=createTask(head);
                    break;
                case 2:
                    readTask(head);
                    break;
                case 3:
                    head=updateTaskName(head);
                    break;
                case 4:
                    head=changeStatus(head);
                    break;
                case 5:
                    head=deleteTask(head);
                    break;
                default:
                    return;
                }
            }
        }
};

int main()
{
    SetConsoleOutputCP(65001);
    Solution s;
    s.ToDo();
}
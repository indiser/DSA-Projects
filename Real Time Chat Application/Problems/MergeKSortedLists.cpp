#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;

    ListNode(int _val)
    {
        val = _val;
        next = nullptr;
    }
};

struct CompareNode {
    bool operator()(ListNode* const& p1, ListNode* const& p2) {
        return p1->val > p2->val;
    }
};

class Solution
{
    public:
        ListNode *mergeKLists(vector<ListNode*> &lists)
        {
            priority_queue<ListNode*, vector<ListNode*>, CompareNode> minHeap;

            for(auto l: lists)
                if(l) minHeap.push(l);
            
            ListNode *dummy = new ListNode(0);
            ListNode *tail = dummy;

            while(!minHeap.empty())
            {
                ListNode *curr = minHeap.top();
                minHeap.pop();
                tail->next = curr;
                tail = tail->next;
                if(curr->next) minHeap.push(curr->next);
            }
            return dummy->next;
        }
        void printList(ListNode* head) {
            while(head) {
                cout << head->val << " ";
                head = head->next;
            }
            cout << endl;
        }
};

int main()
{
    ListNode* l1 = new ListNode(1);
    l1->next = new ListNode(4);
    l1->next->next = new ListNode(5);

    ListNode* l2 = new ListNode(1);
    l2->next = new ListNode(3);
    l2->next->next = new ListNode(4);

    ListNode* l3 = new ListNode(2);
    l3->next = new ListNode(6);

    vector<ListNode*> lists = {l1, l2, l3};

    Solution sol;
    ListNode* result = sol.mergeKLists(lists);

    sol.printList(result);

    return 0;
}
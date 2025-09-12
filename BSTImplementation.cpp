#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// --- ANSI Color Codes ---
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";

// Time Complexity:O(logn)
// Space Complexity:O(n)+O(h) [n=nodes of the tree, h=height of the recursion stack]

struct Tree
{
    int data;
    Tree *left;
    Tree *right;
    Tree()
    {
        data = 0;
        left = nullptr;
        right = nullptr;
    }
    Tree(int x)
    {
        data = x;
        left = nullptr;
        right = nullptr;
    }
};

class BST
{
private:
    Tree *root{nullptr};

    void printTreeVisual(Tree *node, int space)
    {
        if (node == nullptr)
            return;

        space += 10;

        printTreeVisual(node->right, space);

        cout << endl;
        for (int i = 10; i < space; i++)
            cout << " ";
        cout << node->data << "\n";

        printTreeVisual(node->left, space);
    }

    Tree *getSucceesor(Tree *root)
    {
        root = root->right;

        while (root != nullptr && root->left != nullptr)
        {
            root = root->left;
        }

        return root;
    }

    int maxHeight(Tree *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int leftHeight = maxHeight(root->left);
        int rightHeight = maxHeight(root->right);
        return 1 + max(leftHeight, rightHeight);
    }

    Tree *insertNode(Tree *root, int key)
    {
        if (root == nullptr)
        {
            root = new Tree(key);
            return root;
        }

        if (key < root->data)
        {
            root->left = insertNode(root->left, key);
        }
        else if (key > root->data)
        {
            root->right = insertNode(root->right, key);
        }
        return root;
    }
    void inOrderTraversal(Tree *root)
    {
        if (root == nullptr)
        {
            return;
        }

        inOrderTraversal(root->left);
        cout << root->data << " ";
        inOrderTraversal(root->right);
    }
    void preOrderTraversal(Tree *root)
    {
        if (root == nullptr)
        {
            return;
        }

        cout << root->data << " ";
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
    void postOrderTraversal(Tree *root)
    {
        if (root == nullptr)
        {
            return;
        }

        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        cout << root->data << " ";
    }

    bool searchNode(Tree *root, int key)
    {
        if (root == nullptr)
        {
            return false;
        }

        if (root->data == key)
        {
            return true;
        }
        if (key < root->data)
        {
            return searchNode(root->left, key);
        }
        else
        {
            return searchNode(root->right, key);
        }
    }
    Tree *deleteNode(Tree *root, int key)
    {
        if (root == nullptr)
        {
            return root;
        }

        if (key < root->data)
        {
            root->left = deleteNode(root->left, key);
        }
        else if (key > root->data)
        {
            root->right = deleteNode(root->right, key);
        }
        else
        {
            if (root->left == nullptr)
            {
                Tree *temp = root->right;
                delete root;
                return temp;
            }

            if (root->right == nullptr)
            {
                Tree *temp = root->left;
                delete root;
                return temp;
            }

            Tree *succ = getSucceesor(root);
            root->data = succ->data;
            root->right = deleteNode(root->right, succ->data);
        }
        return root;
    }

public:
    void insert(int key)
    {
        root = insertNode(root, key);
    }

    void run()
    {
        while (true)
        {
            int ch;
            cout << BOLD << CYAN << "========================================" << RESET << endl;
            cout << BOLD << CYAN << "      BINARY SEARCH TREE MENU" << RESET << endl;
            cout << BOLD << CYAN << "========================================" << RESET << endl;
            cout << "1. Insert A Node" << endl;
            cout << "2. In Order Traversal The Tree" << endl;
            cout << "3. Pre Order Traversal The Tree" << endl;
            cout << "4. Post Order Traversal Of The Tree" << endl;
            cout << "5. Seach For A Node" << endl;
            cout << "6. Delete A Node" << endl;
            cout << "7. Max Height Of Binary Search Tree" << endl;
            cout << "8. Display The Tree Visualy" << endl;
            cout << YELLOW << "Enter Your Choice: " << RESET;
            cin >> ch;
            cout << "----------------------------------------" << endl;

            switch (ch)
            {
            case 1:
                int key;
                cout << YELLOW << "Enter The Key: " << RESET; // Yellow prompt
                cin >> key;
                root = insertNode(root, key);
                cout << GREEN << "Node " << key << " inserted successfully!" << RESET << endl;
                break;
            case 2:
                if (root == nullptr)
                {
                    cout << RED << "The Tree Is Empty!!" << RESET << endl; // Red error
                }
                else
                {
                    cout << CYAN << "InOrder: " << RESET; // Cyan label
                    inOrderTraversal(root);
                    cout << endl;
                }
                break;
            case 3:
                if (root == nullptr)
                {
                    cout << RED << "The Tree Is Empty!!" << RESET << endl; // Red error
                }
                else
                {
                    cout << CYAN << "InOrder: " << RESET; // Cyan label
                    preOrderTraversal(root);
                    cout << endl;
                }
                break;
            case 4:
                if (root == nullptr)
                {
                    cout << RED << "The Tree Is Empty!!" << RESET << endl;
                }
                else
                {
                    cout << CYAN << "Post-Order Traversal: " << RESET;
                    postOrderTraversal(root);
                    cout << endl;
                }
                break;
            case 5:
                if (root == nullptr)
                {
                    cout << RED << "The Tree Is Empty!!" << RESET << endl;
                }
                else
                {
                    int searchKey;
                    cout << YELLOW << "Enter The Key to Search: " << RESET;
                    cin >> searchKey;
                    if (searchNode(root, searchKey))
                    {
                        cout << GREEN << "Key " << searchKey << " was found!" << RESET << endl;
                    }
                    else
                    {
                        cout << RED << "Key " << searchKey << " was not found." << RESET << endl;
                    }
                }
                break;
            case 6:
                if (root == nullptr)
                {
                    cout << RED << "The Tree Is Empty!!" << RESET << endl;
                }
                else
                {
                    int deleteKey;
                    cout << YELLOW << "Enter The Key to Delete: " << RESET;
                    cin >> deleteKey;
                    // Check if the node exists before confirming deletion
                    if (searchNode(root, deleteKey))
                    {
                        root = deleteNode(root, deleteKey);
                        cout << GREEN << "Node " << deleteKey << " deleted successfully!" << RESET << endl;
                    }
                    else
                    {
                        cout << RED << "Node " << deleteKey << " not found in the tree." << RESET << endl;
                    }
                }
                break;
            case 7:
                if (root == nullptr)
                {
                    cout << "The Tree Is Empty!!" << endl;
                }
                else
                {
                    int height = maxHeight(root);
                    cout << "Max Height Of BST Is: " << height << endl;
                }
                break;
            case 8:
                if (root == nullptr)
                {
                    if (root == nullptr)
                    {
                        cout << RED << "The Tree Is Empty!!" << RESET << endl;
                    }
                    else
                    {
                        int height = maxHeight(root);
                        cout << CYAN << "Max Height Of BST Is: " << RESET << height << endl;
                    }
                    break;
                }
                else
                {
                    cout << "----------------------------------------" << endl;
                    cout << "           VISUAL TREE" << endl;
                    cout << "----------------------------------------" << endl;
                    printTreeVisual(root, 0);
                    cout << "----------------------------------------" << endl;
                }
                break;
            default:
                return;
            }
            cout << "\n"
                 << BOLD << "Press Enter to continue..." << RESET;
            cin.ignore();
            cin.get();
        }
    }
};

int main()
{
    BST bst;

    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    cout << "BST created with initial values: 50, 30, 70, 20, 40, 60, 80" << endl;

    bst.run();

    return 0;
}
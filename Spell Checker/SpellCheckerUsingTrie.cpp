#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;

// Time Complexity:O(L^2 * 26)
// Space Complexity:O(n+L^2 * 26)

class TrieNode
{
public:
    bool isEndOfWord;
    unordered_map<char, TrieNode *> mp;
    TrieNode()
    {
        isEndOfWord = false;
    }
};

class Trie
{
private:
    TrieNode *root;
    void clear(TrieNode *root)
    {
        if (root == nullptr)
            return;

        for (auto const &pair : root->mp)
        {
            clear(pair.second);
        }
        delete root;
    }

public:
    Trie() { root = new TrieNode(); }
    ~Trie()
    {
        clear(root);
    }

    // insert
    void insertTrie(const string &word)
    {
        TrieNode *crawler = root;
        for (char c : word)
        {
            if (crawler->mp.find(c) == crawler->mp.end())
            {
                crawler->mp[c] = new TrieNode();
            }
            crawler = crawler->mp[c];
        }
        crawler->isEndOfWord = true;
    }

    // search
    bool searchTrie(const string &word) const
    {
        TrieNode *crawler = root;
        for (char c : word)
        {
            if (crawler->mp.find(c) == crawler->mp.end())
            {
                return false;
            }
            crawler = crawler->mp[c];
        }
        return crawler->isEndOfWord;
    }

    // start with
    bool startWith(const string &word) const
    {
        TrieNode *crawler = root;
        for (char c : word)
        {
            if (crawler->mp.find(c) == crawler->mp.end())
                return false;
            crawler = crawler->mp[c];
        }
        return true;
    }

    // Deletion
    void deleteTrie(const string &word)
    {
        TrieNode *crawler = root;

        for (char c : word)
        {
            if (crawler->mp.find(c) == crawler->mp.end())
            {
                return;
            }
            crawler = crawler->mp[c];
        }
        if (crawler->isEndOfWord == true)
        {
            crawler->isEndOfWord = false;
        }
    }

    // Suggestions
    vector<string> getSuggestions(const string &word)
    {
        unordered_set<string> candidates;
        vector<string> suggestions;
        int len = word.length();

        // Removing One Charecter
        for (int i = 0; i < len; i++)
        {
            string temp = word;
            temp.erase(i, 1);
            candidates.insert(temp);
        }

        // Transposition
        for (int i = 0; i < len - 1; i++)
        {
            string temp = word;
            swap(temp[i], temp[i + 1]);
            candidates.insert(temp);
        }

        // Substitution
        for (int i = 0; i < len; i++)
        {
            string temp = word;
            for (char c = 'a'; c <= 'z'; c++)
            {
                temp[i] = c;
                candidates.insert(temp);
            }
        }

        // Insertion
        for (int i = 0; i <= len; i++)
        {
            for (char c = 'a'; c <= 'z'; c++)
            {
                string temp = word;
                temp.insert(i, 1, c);
                candidates.insert(temp);
            }
        }

        for (const string &candidate : candidates)
        {
            if (isSpelledCorrectly(candidate))
            {
                suggestions.push_back(candidate);
            }
        }

        return suggestions;
    }

    // AutoCompletion
    void findWords(const TrieNode *node, string prefix, vector<string> &ans)
    {
        if (node->isEndOfWord)
        {
            ans.push_back(prefix);
        }

        for (auto const &pair : node->mp)
        {
            findWords(pair.second, prefix + pair.first, ans);
        }
    }
    vector<string> autoComplete(const string &prefix)
    {
        TrieNode *crawler = root;
        for (char c : prefix)
        {
            if (crawler->mp.find(c) == crawler->mp.end())
                return {};
            crawler = crawler->mp[c];
        }

        vector<string> complete;
        findWords(crawler, prefix, complete);
        return complete;
    }

    // File I/O
    bool populateFromDictionary(const string &filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Error Occurued" << endl;
            return false;
        }

        string word;

        while (getline(file, word))
        {
            if (!word.empty())
            {
                insertTrie(word);
            }
        }

        file.close();
        return true;
    }

    void writeToDictionary(const string &filename, string input)
    {
        ofstream file(filename, ios::app);

        if (!file.is_open())
        {
            cout << "Error Occurued" << endl;
            return;
        }
        file << input << endl;
    }
    bool isSpelledCorrectly(const string &word) const
    {
        return searchTrie(word);
    }
};

int main()
{
    SetConsoleOutputCP(65001);
    Trie trie;

    string filename;
    cout << "Enter The FileName: " << endl;
    getline(cin, filename);

    if (!trie.populateFromDictionary(filename))
        return -1;
    cout << "Smart Spell Checker Ready!" << endl;
    cout << "Dictionary loaded successfully." << endl;
    cout << "Type words or prefixes (type 'exit' to quit):" << endl;

    string input;
    while (true)
    {
        cout << "\n> ";
        getline(cin, input);

        if (input == "exit")
            break;
        if (input.empty())
            continue;

        // Check if it's a complete word first
        if (trie.isSpelledCorrectly(input))
        {
            cout << "✓ '" << input << "' is spelled correctly!" << endl;
        }
        // If not a complete word, check for prefix matches
        else if (trie.startWith(input))
        {
            cout << "Auto-completions for '" << input << "':" << endl;
            vector<string> completions = trie.autoComplete(input);
            for (const string &word : completions)
            {
                cout << "  - " << word << endl;
            }
        }
        // If neither complete word nor valid prefix, suggest corrections
        else
        {
            cout << "✗ '" << input << "' not found." << endl;
            vector<string> suggestions = trie.getSuggestions(input);
            if (!suggestions.empty())
            {
                cout << "Did you mean:" << endl;
                for (const string &suggestion : suggestions)
                {
                    cout << "  - " << suggestion << endl;
                }
            }
            else
            {
                cout << "No suggestions found. Add to dictionary? (y/n): ";
                char choice;
                cin >> choice;
                cin.ignore();
                if (choice == 'y' || choice == 'Y')
                {
                    trie.writeToDictionary(filename, input);
                    trie.insertTrie(input);
                    cout << "'" << input << "' added to dictionary!" << endl;
                }
            }
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
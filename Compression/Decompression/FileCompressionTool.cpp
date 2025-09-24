#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <queue>
#include <limits>
#include <iomanip>
#include <sstream>
using namespace std;

struct TreeNode
{
    char data;
    unsigned int freq;
    TreeNode *right, *left;

    TreeNode(char d, unsigned int f)
    {
        data = d;
        freq = f;
        right = nullptr;
        left = nullptr;
    }
};

struct CompareNodes
{
    bool operator()(TreeNode *left, TreeNode *right)
    {
        if (left->freq != right->freq)
            return left->freq > right->freq;
        return left->data > right->data;
    }
};

class Solution
{
private:
    void deleteTree(TreeNode *node)
    {
        if (node == nullptr)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    unordered_map<char, int> loadFile(const string &filename)
    {
        unordered_map<char, int> freq;
        ifstream file(filename, ios::binary);

        if (!file.is_open())
        {
            cout << "Error Occurred" << endl;
            return {};
        }

        char c;

        while (file.get(c))
        {
            freq[c]++;
        }

        file.close();
        return freq;
    }

    void generateCodes(TreeNode *node, string currentString, unordered_map<char, string> &codeMaps)
    {
        if (node == nullptr)
            return;

        if (!node->left && !node->right)
        {
            codeMaps[node->data] = currentString.empty() ? "0" : currentString;
            return;
        }

        generateCodes(node->left, currentString + '0', codeMaps);
        generateCodes(node->right, currentString + '1', codeMaps);
    }

public:
    void compress(const string &filename, const string &outputFile)
    {
        unordered_map<char, int> mp = loadFile(filename);

        if (mp.empty())
        {
            cout << "Error: Input file is empty or could not be read." << endl;
            return;
        }
        priority_queue<TreeNode *, vector<TreeNode *>, CompareNodes> minHeap;

        for (const auto &pair : mp)
        {
            TreeNode *head = new TreeNode(pair.first, pair.second);
            minHeap.push(head);
        }

        if (minHeap.size() == 1)
        {
            TreeNode *single = minHeap.top();
            TreeNode *newRoot = new TreeNode('\0', single->freq);
            newRoot->left = single;
            newRoot->right = nullptr;
            minHeap.pop();
            minHeap.push(newRoot);
        }

        while (minHeap.size() > 1)
        {
            TreeNode *l = minHeap.top();
            minHeap.pop();

            TreeNode *r = minHeap.top();
            minHeap.pop();

            TreeNode *newNode = new TreeNode('\0', l->freq + r->freq);
            newNode->left = l;
            newNode->right = r;

            minHeap.push(newNode);
        }

        TreeNode *root = minHeap.top();
        unordered_map<char, string> codeMaps;
        generateCodes(root, "", codeMaps);

        long long total_chars = 0;
        for (const auto &pair : mp)
        {
            total_chars += pair.second;
        }

        ofstream outFile(outputFile, ios::binary);
        if (!outFile.is_open())
        {
            cout << "Error: Could not create output file " << outputFile << endl;
            deleteTree(root);
            return;
        }

        int unique_chars = mp.size();
        outFile << total_chars << endl;
        outFile << unique_chars << endl;

        for (const auto &pair : mp)
        {
            outFile << (int)(unsigned char)pair.first << ":" << pair.second << endl;
        }
        
        // Store Huffman codes
        outFile << "CODES" << endl;
        for (const auto &pair : codeMaps)
        {
            outFile << (int)(unsigned char)pair.first << ":" << pair.second << endl;
        }

        streampos valid_bits_pos = outFile.tellp();
        outFile << "000" << endl;

        ifstream inFile(filename, ios::binary);

        if (!inFile.is_open())
        {
            cout << "Error: Could not reopen input file for compression." << endl;
            outFile.close();
            deleteTree(root);
            return;
        }

        unsigned char buffer = 0;
        int bits_in_buffer = 0;
        char c;

        while (inFile.get(c))
        {
            string code = codeMaps[c];

            for (char bit : code)
            {
                buffer = (buffer << 1) | (bit == '1' ? 1 : 0);
                bits_in_buffer++;
                if (bits_in_buffer == 8)
                {
                    outFile.put(buffer);
                    buffer = 0;
                    bits_in_buffer = 0;
                }
            }
        }

        int valid_bits = bits_in_buffer == 0 ? 8 : bits_in_buffer;
        if (bits_in_buffer > 0)
        {
            buffer <<= (8 - bits_in_buffer);
            outFile.put(buffer);
        }

        outFile.seekp(valid_bits_pos);
        outFile << setfill('0') << setw(3) << valid_bits << endl;

        inFile.close();
        outFile.close();
        deleteTree(root);

        cout << "Compression successful. Original size: " << total_chars
             << " characters, unique characters: " << unique_chars << endl;
    }

    

    void decompress(const string &inputFile, const string &outputFile)
    {
        ifstream inFile(inputFile, ios::binary);
        unordered_map<char, int> freqMap;

        if (!inFile.is_open())
        {
            cout << "Could not open input file." << endl;
            return;
        }

        long long total_chars;
        int unique_chars;
        string line;
        getline(inFile, line);
        total_chars = stoll(line);
        getline(inFile, line);
        unique_chars = stoi(line);

        for (int i = 0; i < unique_chars; i++)
        {
            getline(inFile, line);
            if (line.empty())
            {
                i--;
                continue;
            }

            size_t colon_pos = line.find(':');
            if (colon_pos == string::npos)
            {
                cout << "Corrupted or invalid compressed file header." << endl;
                return;
            }
            try
            {
                int ascii_code = stoi(line.substr(0, colon_pos));
                int frequency = stoi(line.substr(colon_pos + 1));
                char character = static_cast<char>(ascii_code);
                freqMap[character] = frequency;
            }
            catch (const exception &e)
            {
                cout << "Error: Invalid data in compressed file header." << endl;
                inFile.close();
                return;
            }
        }

        if (freqMap.empty()) {
            cout << "Error: No character data found in compressed file." << endl;
            inFile.close();
            return;
        }

        // Read Huffman codes
        unordered_map<string, char> codeToChar;
        getline(inFile, line);
        if (line != "CODES") {
            cout << "Error: Missing codes section in compressed file." << endl;
            inFile.close();
            return;
        }
        
        for (const auto &pair : freqMap)
        {
            getline(inFile, line);
            size_t colon_pos = line.find(':');
            if (colon_pos == string::npos) continue;
            
            int ascii_code = stoi(line.substr(0, colon_pos));
            string code = line.substr(colon_pos + 1);
            char character = static_cast<char>(ascii_code);
            codeToChar[code] = character;
        }
        
        TreeNode *root = new TreeNode('\0', 0); // Dummy root
        
        ofstream outFile(outputFile, ios::binary);
        if (!outFile.is_open()) {
            cout << "Error: Could not create output file " << outputFile << endl;
            deleteTree(root);
            inFile.close();
            return;
        }

        long long decoded_count = 0;
        unsigned char buffer;

        int valid_bits;
        getline(inFile, line);
        valid_bits = stoi(line);

        vector<unsigned char> compressed_data;
        while (inFile.get(reinterpret_cast<char&>(buffer)))
        {
            compressed_data.push_back(buffer);
        }

        string current_code = "";
        for (size_t byte_idx = 0; byte_idx < compressed_data.size() && decoded_count < total_chars; byte_idx++)
        {
            buffer = compressed_data[byte_idx];
            bool isLastByte = (byte_idx == compressed_data.size() - 1);
            int bits_to_process = isLastByte ? valid_bits : 8;

            for (int i = 7; i >= 8 - bits_to_process; i--)
            {
                if (decoded_count >= total_chars)
                    break;

                int bit = (buffer >> i) & 1;
                current_code += (bit == 0) ? '0' : '1';

                if (codeToChar.find(current_code) != codeToChar.end())
                {
                    outFile.put(codeToChar[current_code]);
                    decoded_count++;
                    current_code = "";
                }
            }
        }

        
        outFile.close();
        inFile.close();
        delete root;

        cout << "Decompression successful. Decoded " << decoded_count << " characters." << endl;
    }
};

int main()
{
    Solution sol;
    int choice;
    string inputFile, outputFile;

    cout << "File Compression Tool\n";
    cout << "1. Compress\n2. Decompress\nChoice: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer

    cout << "Input file: ";
    getline(cin, inputFile);
    cout << "Output file: ";
    getline(cin, outputFile);

    if (choice == 1)
    {
        sol.compress(inputFile, outputFile);
    }
    else if (choice == 2)
    {
        sol.decompress(inputFile, outputFile);
    }
    else
    {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}

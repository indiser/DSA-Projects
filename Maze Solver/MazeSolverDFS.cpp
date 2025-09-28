#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

// Time Complexity:O(4^(mxn))
// Space Complexity:O(Pxmxn)

class Solution
{
    public:
        void helper(vector<string> &maze,int row, int col, vector<vector<string>> &ans)
        {
            if(row < 0 || row >= maze.size() || col < 0 || col >= maze[0].size() || maze[row][col]=='#')
            {
                return;
            }

            if(maze[row][col]=='E')
            {
                ans.push_back(maze);
                return;
            }

            if(maze[row][col]=='*') return;

            char original=maze[row][col];
            if(original != 'S') {
                maze[row][col]='*';
            }
            helper(maze, row, col + 1, ans);
            helper(maze, row, col - 1, ans);
            helper(maze, row + 1, col, ans);
            helper(maze, row - 1, col, ans);
            if(original != 'S') {
                maze[row][col]=original;
            }
        }
        vector<vector<string>> solveMazeDFS(vector<string> &maze)
        {
            int row=maze.size(), col=maze[0].size();
            int startRow=-1, startCol=-1;
            vector<vector<string>> ans;
            bool foundStart=false;
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if(maze[i][j]=='S')
                    {
                        startRow=i;
                        startCol=j;
                        foundStart=true;
                        break;
                    }
                }
                if(foundStart) break;
            }

            if(foundStart) helper(maze,startRow,startCol,ans);
            return ans;
        }
};

int main()
{
    vector<string> maze = {
        "S.......#",
        "#.#####.#",
        "#.....#.#",
        "#.###.#.#",
        "#...#...#",
        "###.#.#.#",
        "#...#.#.#",
        "#.#...#.#",
        "#.......E"
    };

    // --- Key ---
    // 'S': Start point
    // 'E': End point
    // '.': Open path
    // '#': Wall

    Solution s;
    vector<vector<string>> ans=s.solveMazeDFS(maze);

    cout << "Original Maze:" << endl;
    for(const auto& row : maze)
    {
        cout << row << endl;
    }
    
    cout << "\nNumber of solutions found: " << ans.size() << endl;
    
    for(int i = 0; i < ans.size(); i++)
    {
        cout << "\nSolution " << (i+1) << ":" << endl;
        for(const auto& row : ans[i])
        {
            cout << row << endl;
        }
    }
}
#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
using namespace std;

// Dfs Approach
// Time Complexity: O(m * n) where m = rows, n = columns
// Space Complexity: O(m * n) for visited array + O(min(m,n)) for BFS queue

// class Solution
// {
//     public:
//         void solver(vector<vector<char>> &grid, int row, int col, vector<vector<bool>> &visited)
//         {
//             if(row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() || visited[row][col]==true || grid[row][col]=='0')
//                 return;
//             visited[row][col]=true;
//             solver(grid,row+1,col,visited);
//             solver(grid,row-1,col,visited);
//             solver(grid,row,col+1,visited);
//             solver(grid,row,col-1,visited);
//         }
//         int numberOfIslands(vector<vector<char>> &grid)
//         {
//             int row=grid.size();
//             int col=grid[0].size();
//             vector<vector<bool>> visited(row,vector<bool>(col,false));
//             int count=0;
//             for (int i = 0; i < row; i++)
//             {
//                 for (int j = 0; j < col; j++)
//                 {
//                     if(grid[i][j]=='1' && visited[i][j]==false)
//                     {
//                         solver(grid,i,j,visited);
//                         count++;
//                     }  
//                 }
//             }
//             return count;
//         }
// };

// BFS Approach
// Time Complexity: O(m * n) - visit each cell once in nested loops + BFS visits each '1' cell once
// Space Complexity: O(m * n) - visited array O(m*n) + BFS queue O(min(m,n)) in worst case

class Solution
{
    public:
        void bfs(vector<vector<char>> &grid, int startRow, int startCol, vector<vector<bool>> &visited)
        {
            queue<pair<int,int>> q;
            q.push({startRow, startCol});
            visited[startRow][startCol] = true;
            
            int dr[] = {-1, 0, 1, 0};
            int dc[] = {0, 1, 0, -1};
            
            while(!q.empty())
            {
                pair<int,int> curr = q.front();
                q.pop();
                
                for(int i = 0; i < 4; i++)
                {
                    int newRow = curr.first + dr[i];
                    int newCol = curr.second + dc[i];
                    
                    if(newRow >= 0 && newRow < grid.size() && newCol >= 0 && newCol < grid[0].size() && 
                       grid[newRow][newCol] == '1' && !visited[newRow][newCol])
                    {
                        visited[newRow][newCol] = true;
                        q.push({newRow, newCol});
                    }
                }
            }
        }
        
        int numberOfIslands(vector<vector<char>> &grid)
        {
            int row = grid.size();
            int col = grid[0].size();
            vector<vector<bool>> visited(row, vector<bool>(col, false));
            int count = 0;
            
            for(int i = 0; i < row; i++)
            {
                for(int j = 0; j < col; j++)
                {
                    if(grid[i][j] == '1' && !visited[i][j])
                    {
                        bfs(grid, i, j, visited);
                        count++;
                    }
                }
            }
            return count;
        }
};


int main()
{
    vector<vector<char>> grid={
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };
    
    Solution s;
    cout << s.numberOfIslands(grid) << endl;
    return 0;
}

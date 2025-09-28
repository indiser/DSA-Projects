#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
#include<utility>
using namespace std;

// Time Complexity:O(mxn)
// Space Complexity:O(mxn)

class Solution
{
    public:
        void solveMazeBFS(vector<string> &maze)
        {
            int row=maze.size(), col=maze[0].size();
            bool foundStart=false;

            pair<int,int> start={-1, -1};
            pair<int,int> end={-1, -1};
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if(maze[i][j]=='S')
                        start={i,j};
                    if(maze[i][j]=='E')
                        end={i,j};
                }
            }
            
            if(start.first==-1) {
                cout << "Start point not found!" << endl;
                return;
            }
            
            cout << "Start: (" << start.first << "," << start.second << ")" << endl;
            cout << "End: (" << end.first << "," << end.second << ")" << endl;

            queue<pair<int,int>> q;
            vector<vector<bool>> visited(row, vector<bool>(col, false));
            vector<vector<pair<int, int>>> parent(row, vector<pair<int, int>>(col, {-1, -1}));

            q.push(start);
            visited[start.first][start.second]=true;
            bool foundEnd=false;

            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};

            while(!q.empty())
            {
                pair<int,int> current=q.front();
                q.pop();

                if(current==end)
                {
                    foundEnd=true;
                    break;
                }

                for (int i = 0; i < 4; i++)
                {
                    int nextRow=current.first+dr[i];
                    int nextCol=current.second+dc[i];

                    if(nextRow >= 0 && nextRow < row && nextCol >= 0 && nextCol < col && (maze[nextRow][nextCol]=='.' || maze[nextRow][nextCol]=='E') && !visited[nextRow][nextCol])
                    {
                        visited[nextRow][nextCol]=true;
                        q.push({nextRow,nextCol});
                        parent[nextRow][nextCol]=current;
                    }
                }
            }

            if(foundEnd)
            {
                pair<int,int> current=end;
                while(current != start)
                {
                    current=parent[current.first][current.second];
                    if(current != start)
                    {
                        maze[current.first][current.second]='*';
                    }
                }
            }
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
    s.solveMazeBFS(maze);

    std::cout << "Solved Maze:" << std::endl;
    for (const auto& row : maze) {
        std::cout << row << std::endl;
    }

    return 0;
}
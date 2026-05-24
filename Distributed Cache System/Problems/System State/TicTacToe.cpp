#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

class TicTacToe
{
    private:
        int boardSize;
        vector<vector<int>> playerCounts;
    public:
        TicTacToe(int size) : boardSize(size), playerCounts(2, vector<int>(2 *size + 2, 0)) {};

        int move(int row, int col, int player)
        {
            vector<int> &currentPlayerCounts = playerCounts[player -1];
            ++currentPlayerCounts[row];
            ++currentPlayerCounts[boardSize + col];

            if(row == col) ++currentPlayerCounts[boardSize * 2];
            if(row + col == boardSize + 1) ++currentPlayerCounts[boardSize *2 + 1];

            if (currentPlayerCounts[row] == boardSize ||
                currentPlayerCounts[boardSize + col] == boardSize ||
                currentPlayerCounts[boardSize * 2] == boardSize ||
                currentPlayerCounts[boardSize * 2 + 1] == boardSize) { 
                    return player;
            }
            return 0;
        }
};

int main()
{
    TicTacToe game(3);
    
    cout << game.move(0, 0, 1) << endl; // Player 1 at (0,0) -> 0
    cout << game.move(0, 2, 2) << endl; // Player 2 at (0,2) -> 0
    cout << game.move(2, 2, 1) << endl; // Player 1 at (2,2) -> 0
    cout << game.move(1, 1, 2) << endl; // Player 2 at (1,1) -> 0
    cout << game.move(2, 0, 1) << endl; // Player 1 at (2,0) -> 0
    cout << game.move(1, 0, 2) << endl; // Player 2 at (1,0) -> 0
    cout << game.move(2, 1, 1) << endl; // Player 1 at (2,1) -> 1 (wins)
    
    return 0;
}
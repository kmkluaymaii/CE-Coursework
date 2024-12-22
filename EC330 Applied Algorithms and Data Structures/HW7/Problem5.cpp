//Rawisara Chairat, rawisara@bu.edu
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility> 

using namespace std;

bool isPossible(vector<vector<int>>& maze, int maze_size, int x, int y){
    // This function check if it is possible to move to the next coordinates
    if((x >= 0) && (x < maze_size) && (y >=0) && (y < maze_size) && (maze[x][y] == 1)){
        return true;
    }
    return false;

}

int BFS(vector<vector<int>>& maze, int maze_size){
    queue<pair<int, int>> q;
    
    // Initially mark all the vertices as not visited
    // When we push a vertex into the q, we mark it as 
    // visited
    vector<vector<bool>> visited(maze_size, vector<bool>(maze_size, false));
    vector<pair<int,int>> move = {{1,0},{-1,0},{0,1},{0,-1}}; //Right, left, down and up

    // Mark the source node as visited and 
    // enqueue it
    visited[0][0] = true;
    q.push({0, 0});

    int path = 1;
    int new_x = 0;
    int new_y = 0;
    int q_size = 0;

    // Iterate over the queue
    while (!q.empty()) {
        q_size = q.size();
        for (int i = 0; i < q_size; ++i) {
        // Dequeue a vertex from queue and print it
            auto [x, y] = q.front();
            q.pop();

            // If it reaches the end of the maze (N-1, N-1), 
            // then return the length of the path
            if((x == maze_size-1) && (y == maze_size-1)){
                return path;
            }

            // Move in every direction and see
            // 1. If it is possible
            // 2. If it has not been visited
            for (auto& mov : move) {
                new_x = x + mov.first;
                new_y = y + mov.second;
                if (isPossible(maze, maze_size, new_x, new_y) && (visited[new_x][new_y] == false)) {
                    visited[new_x][new_y] = true;
                    q.push({new_x, new_y});
                }
            }
        }
        path++;
    }
    return 0;
}

int main(int argc, const char * argv[]){
    ifstream input;
    input.open(argv[1]);
    string maze_line;

    if (!input) {
        return -1;
    }

    int shortest_path = 0;

    vector<vector<int>> maze;
    while(getline(input, maze_line)){
        vector<int> line;
        for (char c : maze_line) {
            if (c == '0' || c == '1') {
                line.push_back(c-48);
            }
        }
        maze.push_back(line);
    }
    
    if(maze.empty()){
        cout << shortest_path << endl;
        return 0;
    }

    shortest_path = BFS(maze, maze.size());
    cout << shortest_path << endl;

    input.close();
    return 0;
}
